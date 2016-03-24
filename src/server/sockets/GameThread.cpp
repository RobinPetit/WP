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
#include <sstream>

constexpr std::chrono::seconds GameThread::_turnTime;

GameThread::GameThread(ServerDatabase& database, userId player1Id, userId player2Id):
	std::thread(),
	_player1Id(player1Id),
	_player2Id(player2Id),
	_running(false),
	_player1(*this, database, _player1Id, _player2, _postGameDataPlayer1),
	_player2(*this, database, _player2Id, _player1, _postGameDataPlayer2),
	_database(database),
	_winnerId{0},
	_turn(0),
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

void GameThread::printVerbose(std::string message)
{
	if (not _verbose)
		return;

	std::istringstream iss("game n " + message);
	std::string line;
	while (std::getline(iss, line))
		std::cout << "\t" << line << std::endl;  //print each line with indentation
}

userId GameThread::playGame(const ClientInformations& player1, const ClientInformations& player2)
{
	setSocket(_player1.getSocket(), _specialOutputSocketPlayer1, player1);
	setSocket(_player2.getSocket(), _specialOutputSocketPlayer2, player2);

	// ask the clients to choose their decks
	_activePlayer->receiveDeck();
	_passivePlayer->receiveDeck();

	// initialize player's data and send "game starting" signal
	_activePlayer->setUpGame(true);
	_passivePlayer->setUpGame(false);

	// start the thread that times out slow players
	_timerThread = std::thread(&GameThread::makeTimer, this);

	// run and time the game
	runGame();

	// unlock a random new card
	cardId earnedCardId{_database.getRandomCardId()};
	if (_postGameDataPlayer1.playerWon) _postGameDataPlayer1.unlockedCard = earnedCardId;
	if (_postGameDataPlayer2.playerWon) _postGameDataPlayer2.unlockedCard = earnedCardId;

	// display player's post game data
	printVerbose(std::string("Player1's Post Game Data : \n") + _postGameDataPlayer1.display());
	printVerbose(std::string("Player2's Post Game Data : \n") + _postGameDataPlayer2.display());

	// send postGameData to database, receive new unlocked achievements
	AchievementList newAchievementsPlayer1 = _database.newAchievements(_postGameDataPlayer1, _player1Id);
	AchievementList newAchievementsPlayer2 = _database.newAchievements(_postGameDataPlayer2, _player2Id);

	// send last message to both players
	sendFinalMessage(_specialOutputSocketPlayer1, _postGameDataPlayer1, earnedCardId, newAchievementsPlayer1);
	sendFinalMessage(_specialOutputSocketPlayer2, _postGameDataPlayer2, earnedCardId, newAchievementsPlayer2);

	return _winnerId;
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

void GameThread::runGame()
{
	// used to calculate time duration of the game
	std::chrono::steady_clock::time_point startOfGame = std::chrono::steady_clock::now();

	// call explicitely enterTurn for the first player because this method
	// is only called when there is a turn swapping. So first turn is never
	// **officially** started
	_activePlayer->enterTurn(1);
	//no need to call leaveTurn for passive Player

	while(_running.load())
	{
		if (_turnSwap.load())
			endTurn();

		for(auto player : {_activePlayer, _passivePlayer})
		{
			auto otherPlayer{ player == _activePlayer ? _passivePlayer : _activePlayer };

			sf::TcpSocket& specialSocket{player == &_player1 ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2};

			auto status{player->tryReceiveClientInput()}; // get input
			// player has disconnected
			if(status == sf::Socket::Disconnected)
			{
				std::cerr << "Lost connection with a player\n";
				//winner is the player who's still connected
				otherPlayer->_postGameData.playerWon = true;
				player->_postGameData.playerRageQuit = true;

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
			}

			// the game has been won/interrupted
			if (_running.load() == false)
				break;
		}
		sf::sleep(sf::milliseconds(50));
	}

	// calculate duration of the game
	std::chrono::steady_clock::time_point endOfGame = std::chrono::steady_clock::now();
	std::size_t gameDuration = static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::seconds>(endOfGame - startOfGame).count());
	_postGameDataPlayer1.gameDuration = _postGameDataPlayer2.gameDuration = gameDuration;

	assert((_winnerId != 0) xor (_endGameCause == EndGame::Cause::ENDING_SERVER));
}

void GameThread::endGame(userId winnerId, EndGame::Cause cause)
{
	std::cout << "Game is asked to end\n";
	_winnerId = winnerId;
	_endGameCause = cause;
	_running.store(false);
}

void GameThread::interruptGame()
{
	// verify the game is ended before to interrupt it
	// (do not end the same game twice)
	if(_running.load())
		endGame(0, EndGame::Cause::ENDING_SERVER);
}

void GameThread::swapTurns()
{
	_turnSwap.store(true);
}

void GameThread::endTurn()
{
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

	_turn++;  // turn counter (for both players)
	_activePlayer->leaveTurn();
	std::swap(_activePlayer, _passivePlayer);  // swap active and inactive
	_activePlayer->enterTurn(_turn/2 +1);  // ALWAYS call active player

	_startOfTurnTime = std::chrono::high_resolution_clock::now();
	_turnSwap.store(false);
}

// Function only called by a new thread
void GameThread::makeTimer()
{
	static const std::chrono::milliseconds sleepingTime{50};
	_startOfTurnTime = std::chrono::high_resolution_clock::now();

	while(_running.load())
	{
		std::this_thread::sleep_for(sleepingTime);

		// if the current player didn't finished his turn and he still has got time to play, wait
		if(!_turnSwap.load() && std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - _startOfTurnTime) < _turnTime)
			continue;

		swapTurns();
	}
}

void GameThread::sendFinalMessage(sf::TcpSocket& specialSocket, PostGameData& postGameData, cardId earnedCardId, AchievementList& newAchievements)
{
	sf::Packet packet;

	// EndGame::applyToSelf indicate which player won the game: false means the player himself won
	if (postGameData.playerWon)  // send card message + unlocked card + new achievements
		packet << TransferType::GAME_OVER << EndGame{_endGameCause, false} << earnedCardId << newAchievements;
	else // if player lost : send message + new achievements
		packet << TransferType::GAME_OVER << EndGame{_endGameCause, true} << newAchievements;
	specialSocket.send(packet);
}

GameThread::~GameThread()
{
	interruptGame();
	if(_timerThread.joinable())
		_timerThread.join();
}
