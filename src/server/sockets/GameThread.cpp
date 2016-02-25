// WizardPoker headers
#include "server/GameThread.hpp"
#include "common/sockets/TransferType.hpp"
// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/System/Sleep.hpp>
// std-C++ headers
#include <iostream>
#include <chrono>

constexpr std::chrono::seconds GameThread::_turnTime;

GameThread::GameThread(Player::ID player1ID, Player::ID player2ID):
	std::thread(),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(false),
	_gameBoard({player1ID, _socketPlayer1, _specialOutputSocketPlayer1}, {player2ID, _socketPlayer2, _specialOutputSocketPlayer2}),
	_isSynchroWithBoard(_player1ID == _gameBoard.getCurrentPlayerID())
{

}

void GameThread::interruptGame()
{
	_running.store(false);
}

void GameThread::establishSockets(const ClientInformations& player1, const ClientInformations& player2)
{
	setSocket(_socketPlayer1, _specialOutputSocketPlayer1, player1);
	setSocket(_socketPlayer2, _specialOutputSocketPlayer2, player2);
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

/////////// ease of implementation

GameThread::PlayerNumber GameThread::PlayerFromID(const Player::ID ID)
{
	return ((ID == _player1ID && _isSynchroWithBoard) ||
	        (ID == _player2ID && !_isSynchroWithBoard))
	         ? PlayerNumber::PLAYER1
	         : PlayerNumber::PLAYER2;
}

sf::TcpSocket& GameThread::getSocketFromID(const Player::ID ID)
{
	return PlayerFromID(ID) == PlayerNumber::PLAYER1 ? _socketPlayer1 : _socketPlayer2;
}

sf::TcpSocket& GameThread::getSpecialSocketFromID(const Player::ID ID)
{
	return PlayerFromID(ID) == PlayerNumber::PLAYER1 ? _specialOutputSocketPlayer1 : _specialOutputSocketPlayer2;
}

void GameThread::startGame(const ClientInformations& player1, const ClientInformations& player2)
{
	establishSockets(player1, player2);
	sf::Packet packet;
	packet << TransferType::GAME_STARTING << TransferType::GAME_PLAYER_ENTER_TURN;
	getSocketFromID(_gameBoard.getCurrentPlayerID()).send(packet);
	packet.clear();
	packet << TransferType::GAME_STARTING << TransferType::GAME_PLAYER_LEAVE_TURN;
	getSocketFromID(_gameBoard.getWaitingPlayerID()).send(packet);
	_timerThread = std::thread(&GameThread::makeTimer, this);
	// prepare data listening
	sf::SocketSelector selector;
	selector.add(_socketPlayer1);
	selector.add(_socketPlayer2);
	while(_running.load())
	{
		GameThread::PlayerNumber sender;
		if(!selector.wait(sf::milliseconds(50)))
			continue;
		if(selector.isReady(_socketPlayer1))
			sender = PlayerNumber::PLAYER1;
		else if(selector.isReady(_socketPlayer2))
			sender = PlayerNumber::PLAYER2;
		else
			throw std::runtime_error("Error with selector while waiting for game data");
		// get actions from playing client
		sf::TcpSocket& modifiedSocket{sender == PlayerNumber::PLAYER1 ? _socketPlayer1 : _socketPlayer2};
		modifiedSocket.receive(packet);
		TransferType type;
		packet >> type;
		if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
			_turnSwap.store(true);
	}
	packet.clear();
	packet << TransferType::GAME_OVER;
	// same message is sent to both players: no need to find by ID
	_specialOutputSocketPlayer1.send(packet);
	_specialOutputSocketPlayer2.send(packet);
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
		sf::Packet endOfTurn;
		endOfTurn << TransferType::GAME_PLAYER_LEAVE_TURN;
		getSpecialSocketFromID(_gameBoard.getCurrentPlayerID()).send(endOfTurn);
		sf::Packet startOfTurn;
		startOfTurn << TransferType::GAME_PLAYER_ENTER_TURN;
		getSpecialSocketFromID(_gameBoard.getWaitingPlayerID()).send(startOfTurn);
		_gameBoard.endTurn();
		startOfTurnTime = std::chrono::high_resolution_clock::now();
		_turnSwap.store(false);
	}
}

GameThread::~GameThread()
{
	interruptGame();
	_timerThread.join();
}
