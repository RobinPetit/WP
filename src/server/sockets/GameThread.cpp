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

constexpr std::chrono::seconds GameThread::_turnTime;

GameThread::GameThread(ServerDatabase& database, userId player1ID, userId player2ID):
	std::thread(),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(false),
	_player1(*this, database, _player1ID),
	_player2(*this, database, _player2ID),
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
	if(std::bernoulli_distribution(0.5)(device))
		std::swap(_activePlayer, _passivePlayer);
	_activePlayer->setOpponent(_passivePlayer);
	_passivePlayer->setOpponent(_activePlayer);
}

// \TODO: complete the function as a QuitGame
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
	sf::Packet packet;
	std::cout << "waiting for decks\n";
	_player1.receiveDeck();
	_player2.receiveDeck();

	// send the game is starting
	_activePlayer->beginGame(true);
	_passivePlayer->beginGame(false);
	_timerThread = std::thread(&GameThread::makeTimer, this);

	userId winnerId{runGame()};

	// Send to the winner he wins and send his new card ID
	packet.clear();
	std::random_device device;
	std::mt19937 generator{device()};
	cardId earnedCardId{std::uniform_int_distribution<int>(0, static_cast<int>(nbSpells + nbCreatures))(generator)};
	++earnedCardId;  // Card indices start to 1 because of SQLite
	_database.addCard(winnerId, earnedCardId);
	// \TODO: add earnedCardId to winner's card collection

	// Send to the winner he won
	// EndGame::applyToSelf indicate which player won the game: false mean that
	// the player who receive this message has won.
	packet << TransferType::GAME_OVER << EndGame{_endGamecause, false} << earnedCardId;
	sf::TcpSocket& winnerSocket{winnerId == _player1ID ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2};
	winnerSocket.send(packet);

	// Send to the loser he lost (do NOT send a card index thus)
	packet.clear();
	packet << TransferType::GAME_OVER << EndGame{_endGamecause, true};
	sf::TcpSocket& loserSocket{winnerId == _player1ID ? _specialOutputSocketPlayer2 : _specialOutputSocketPlayer1};
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

			auto status{player->tryReceiveClientInput()};
			if(status == sf::Socket::Disconnected)
			{
				std::cerr << "Lost connection with a player\n";
				_winner = player->getID();
				_running.store(false);
				break;
			}
			// The player received some valid input from the client
			else if(status == sf::Socket::Done)
			{
				// Send the changes to the client
				if(player->thereAreBoardChanges())
				{
					sf::Packet boardChanges{player->getBoardChanges()};
					specialSocket.send(boardChanges);
				}

				// Check if the player quitted the game
				if(_winner != 0)
				{
					_running.store(false);
					break;
				}
			}
		}
		sf::sleep(sf::milliseconds(250));
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

/*------------------------------ PLAYER AND CARD INTERFACE */
void GameThread::applyEffect(Card* usedCard, EffectParamsCollection effectArgs)
{
	int subject;  // who the effect applies to
	try  // check the input
	{
		subject=effectArgs.at(0);
		effectArgs.erase(effectArgs.begin());
	}
	catch (std::out_of_range&)
	{
		//SERVER: CARD ERROR
		return;
	}

	switch (subject)
	{
		case PLAYER_SELF:	//passive player
			_activePlayer->askUserToSelectCards({});
			_activePlayer->applyEffect(usedCard, effectArgs);
			break;
		case PLAYER_OPPO:	//active player
			_activePlayer->askUserToSelectCards({});
			_passivePlayer->applyEffect(usedCard, effectArgs);
			break;
		case CREATURE_SELF_THIS:	//active player's creature that was used
			_activePlayer->askUserToSelectCards({});
			_activePlayer->applyEffectToCreature(dynamic_cast<Creature*>(usedCard), effectArgs);
			break;
		case CREATURE_SELF_INDX:	//active player's creature at given index
			_activePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->askUserToSelectCards({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_RAND:	//active player's creature at random index
			_activePlayer->askUserToSelectCards({});
			_activePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->getRandomBoardIndexes({CardToSelect::SELF_BOARD}));
			break;

		case CREATURE_SELF_TEAM:	//active player's team of creatures
			_activePlayer->askUserToSelectCards({});
			_activePlayer->applyEffectToCreatureTeam(usedCard, effectArgs);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			_passivePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->askUserToSelectCards({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_RAND:	//passive player's creature at random index
			_activePlayer->askUserToSelectCards({});
			_passivePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->getRandomBoardIndexes({CardToSelect::OPPO_BOARD}));
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			_activePlayer->askUserToSelectCards({});
			_passivePlayer->applyEffectToCreatureTeam(usedCard, effectArgs);
			break;
	}
	throw std::runtime_error("Effect subject not valid");
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
	_timerThread.join();
}
