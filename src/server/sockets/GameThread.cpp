// WizardPoker headers
#include "server/GameThread.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "server/Creature.hpp"
#include "common/CardData.hpp"
// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/System/Sleep.hpp>
// std-C++ headers
#include <iostream>
#include <chrono>
#include <cassert>

extern RandomInteger intGenerator;

constexpr std::chrono::seconds GameThread::_turnTime;

GameThread::GameThread(ServerDatabase& database, userId player1Id, userId player2Id):
	std::thread(),
	_player1Id(player1Id),
	_player2Id(player2Id),
	_running(false),
	_player1(*this, database, _player1Id, _player2),
	_player2(*this, database, _player2Id, _player1),
	_database(database),
	_winner{0},
	_turn(0),
	_turnCanEnd(false),
	_turnSwap{false}
{
	createPlayers();
}

void GameThread::createPlayers()
{
	_activePlayer = &_player1;
	_passivePlayer = &_player2;
	std::random_device device;
	if(std::bernoulli_distribution(0.5)(device)) //Choose which player starts
		std::swap(_activePlayer, _passivePlayer);
}

RandomInteger& GameThread::getGenerator()
{
	return _intGenerator;
}

// \TODO: complete the function as a QuitGame
// \TODO: is the function useful at all ?
void GameThread::interruptGame()
{
	_running.store(false);
	//TODO: need identifier for player who quit
	//NETWORK: PLAYER_QUIT_GAME
}

void GameThread::establishSockets(const ClientInformations& player1, const ClientInformations& player2)
{
	setSocket(_player1.getSocket(), _specialOutputSocketPlayer1, player1);
	setSocket(_player2.getSocket(), _specialOutputSocketPlayer2, player2);
}

void GameThread::setSocket(sf::TcpSocket& socket, sf::TcpSocket& specialSocket, const ClientInformations& player)
{
	sf::TcpSocket tmpSocket;
	tmpSocket.connect(player.socket->getRemoteAddress(), player.listeningPort);
	sf::TcpListener listener;
	listener.listen(sf::Socket::AnyPort);
	sf::Packet packet;
	packet << TransferType::NEW_GAME_SERVER_CONNECTION << listener.getLocalPort();
	tmpSocket.send(packet);
	// Here, the client connects itself
	if(listener.accept(socket) != sf::Socket::Done)
		std::cerr << "Error while creating game thread socket\n";
	// and the "specialSocket" used to send important data which are
	// not blocking the main client thread (eg: END_OF_TURN, BOARD_UPDATE, etc.)
	if(listener.accept(specialSocket) != sf::Socket::Done)
		std::cerr << "Error while creating game thread special socket\n";
}

userId GameThread::startGame(const ClientInformations& player1, const ClientInformations& player2)
{
	establishSockets(player1, player2);
	_player1.receiveDeck();
	_player2.receiveDeck();

	// send the game is starting
	_activePlayer->beginGame(true);
	_passivePlayer->beginGame(false);
	_timerThread = std::thread(&GameThread::makeTimer, this);

	// call explicitely enterTurn for the first player because this method
	// is only called when there is a turn swapping. So first turn is never
	// **officially** started
	_activePlayer->enterTurn(1);

	userId winnerId{runGame()};

	// Send to the winner he wins and send his new card ID
	sf::Packet packet;
	cardId earnedCardId{_intGenerator.next(nbSpells + nbCreatures)};
	++earnedCardId;  // Card indices start to 1 because of SQLite
	_database.addCard(winnerId, earnedCardId);
	// \TODO: change by cardId earnedCardId = _database.unlockNewCard(winnerId) ?

	// Send to the winner he won
	// EndGame::applyToSelf indicate which player won the game: false mean that
	// the player who receive this message has won.
	packet << TransferType::GAME_OVER << EndGame{_endGamecause, false} << earnedCardId;
	sf::TcpSocket& winnerSocket{winnerId == _player1Id ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2};
	winnerSocket.send(packet);

	// Send to the loser he lost (do NOT send a card index thus)
	packet.clear();
	packet << TransferType::GAME_OVER << EndGame{_endGamecause, true};
	sf::TcpSocket& loserSocket{winnerId == _player1Id ? _specialOutputSocketPlayer2 : _specialOutputSocketPlayer1};
	loserSocket.send(packet);

	return winnerId;
}

userId GameThread::runGame()
{
	while(_running.load())
	{
		for(auto player : {_activePlayer, _passivePlayer})
		{
			sf::TcpSocket& specialSocket{player == &_player1 ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2};

			auto status{player->tryReceiveClientInput()}; // get input
			// player has disconnected
			if(status == sf::Socket::Disconnected)
			{
				std::cerr << "Lost connection with a player\n";
				//winner is the player who's still connected
				userId winnerId = player == _activePlayer ? _passivePlayer->getId() : _activePlayer->getId();
				endGame(winnerId, EndGame::Cause::LOST_CONNECTION);
				break;
			}

			// error while transmitting
			if(status == sf::Socket::Error)
				std::cerr << "Error while transmitting, ignoring block\n";

			// received some valid input from the client
			else  // if status == sf::Socket::Done or if status == sf::Socket::NotReady
			{
				// Send the changes to the client
				if(player->thereAreBoardChanges())
				{
					sf::Packet boardChanges{player->getBoardChanges()};
					specialSocket.send(boardChanges);
				}

				// Check if the player quitted the game \TODO: when is this useful ??
				if(_winner != 0)
				{
					_running.store(false);
					break;
				}
			}
		}
		sf::sleep(sf::milliseconds(50));
	}
	assert(_winner != 0);
	return _winner;
}

void GameThread::endGame(userId winnerId, EndGame::Cause cause)
{
	std::cout << "Game is asked to end\n";
	_winner = winnerId;
	_endGamecause = cause;
	_running.store(false);
}

void GameThread::endTurn()
{
	_turn++;  // turn counter (for both players)
	_activePlayer->leaveTurn();
	std::swap(_activePlayer, _passivePlayer);  // swap active and inactive
	_activePlayer->enterTurn(_turn/2 +1);  // ALWAYS call active player
}

void GameThread::swapTurns()
{
	_turnSwap.store(true);
}

// Function only called by a new thread
void GameThread::makeTimer()
{
	static const std::chrono::milliseconds sleepingTime{50};
	auto startOfTurnTime{std::chrono::high_resolution_clock::now()};
	while(_running.load())
	{
		std::this_thread::sleep_for(sleepingTime);
		// if the current player didn't finished his turn and he still has got time to play, wait
		if(!_turnSwap.load() && std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startOfTurnTime) < _turnTime)
			continue;
		// send to both players their turn swapped
		// MAYBE TODO: store and maintain two pointers, _activeSpecialSocket and _passiveSpecialSocket as attributes
		sf::TcpSocket& activeSpecialSocket{_activePlayer == &_player1 ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2};
		sf::Packet endOfTurn;
		endOfTurn << TransferType::GAME_PLAYER_LEAVE_TURN;
		activeSpecialSocket.send(endOfTurn);

		sf::TcpSocket& passiveSpecialSocket{_activePlayer == &_player1 ? _specialOutputSocketPlayer2 : _specialOutputSocketPlayer1};
		sf::Packet startOfTurn;
		startOfTurn << TransferType::GAME_PLAYER_ENTER_TURN;
		passiveSpecialSocket.send(startOfTurn);

		endTurn();
		startOfTurnTime = std::chrono::high_resolution_clock::now();
		_turnSwap.store(false);
	}
}

GameThread::~GameThread()
{
	interruptGame();
	if(_timerThread.joinable())
		_timerThread.join();
}
