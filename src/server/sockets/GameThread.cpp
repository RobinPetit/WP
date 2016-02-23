// WizardPoker headers
#include "server/GameThread.hpp"
#include "common/sockets/TransferType.hpp"
// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
// std-C++ headers
#include <iostream>

GameThread::GameThread(Player::ID player1ID, Player::ID player2ID):
	std::thread(),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(false),
	_gameBoard(player1ID, player2ID, _socketPlayer1, _socketPlayer2),
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
	packet << TransferType::GAME_STARTING << TransferType::GAME_PLAYER_LEAVING_TURN;
	getSocketFromID(_gameBoard.getWaitingPlayerID()).send(packet);
	/// \TODO main loop of the game
	packet.clear();
	packet << TransferType::GAME_OVER;
	// same message is sent to both players: no need to find by ID
	_specialOutputSocketPlayer1.send(packet);
	_specialOutputSocketPlayer2.send(packet);
}
