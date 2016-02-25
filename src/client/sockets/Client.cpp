// SFML headers
#include <SFML/System/Sleep.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/ErrorCode.hpp"
#include "common/constants.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/NotConnectedException.hpp"
#include "common/sockets/PacketOverload.hpp"
// std-C++ headers
#include <iostream>
#include <thread>
#include <algorithm>
#include <atomic>
#include <string>

Client::Client():
	_chatListenerPort(0),
	_isConnected(false),
	_serverPort(0),
	_threadLoop(false),
	_inGame(false),
	_readyToPlay(false)
{

}

bool Client::connectToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port)
{
	return initServer(name, address, port) && sendConnectionToken(password);
}

bool Client::registerToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port)
{
	std::string shrinkedName = shrinkName(name);
	// The local socket used only to register, it does not need to be keeped as attribute
	sf::TcpSocket socket;
	// if connection does not work, don't go further
	if(socket.connect(address, port) != sf::Socket::Done)
		return false;
	return sendRegisteringToken(shrinkedName, password, socket);
}

bool Client::sendConnectionToken(const std::string& password)
{
	sf::Packet packet;
	packet << TransferType::GAME_CONNECTION
	       << _name
	       << password
	       << static_cast<sf::Uint16>(_chatListenerPort);
	if(_socket.send(packet) != sf::Socket::Done)
	{
		std::cout << "Failed to send connection packet\n";
		return false;
	}

	// Receive the server response
	_socket.receive(packet);
	TransferType response;
	packet >> response;
	switch(response)
	{
	case TransferType::GAME_ALREADY_CONNECTED:
		//TODO throw an exception rather than cout
		std::cout << "Error: you are already connected!\n";
		return false;

	case TransferType::GAME_WRONG_IDENTIFIERS:
		std::cout << "Error: invalid username or password.\n";
		return false;

	case TransferType::GAME_CONNECTION_OR_REGISTERING_OK:
		_isConnected = true;
		updateFriends();
		return true;

	default:
		std::cout << "Error: unidentified server response.\n";
		return false;
	}
}
bool Client::sendRegisteringToken(const std::string& name, const std::string& password, sf::TcpSocket& socket)
{
	sf::Packet packet;
	packet << TransferType::GAME_REGISTERING << name << password;
	if(socket.send(packet) != sf::Socket::Done)
	{
		std::cout << "sending packet failed";
		return false;
	}

	// Receive the server response
	socket.receive(packet);
	TransferType response;
	packet >> response;
	switch(response)
	{
	case TransferType::GAME_USERNAME_NOT_AVAILABLE:
		//TODO throw an exception rather than cout
		std::cout << "Error: the username " << name << " is not available\n";
		return false;

	case TransferType::GAME_FAILED_TO_REGISTER:
		std::cout << "Error: the server failed to register your account.\n";
		return false;

	case TransferType::GAME_CONNECTION_OR_REGISTERING_OK:
		return true;

	default:
		std::cout << "Error: unidentified server response.\n";
		return false;
	}
}

bool Client::initServer(const std::string& name, const sf::IpAddress& address, sf::Uint16 port)
{
	// if client is already connected to a server, do not try to re-connect it
	if(_isConnected)
	{
		std::cout << "Already connected\n";
		return false;
	}
	_name = shrinkName(name);
	_serverAddress = address;
	_serverPort = port;
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
	{
		std::cout << "Failed to establish socket connection\n";
		return false;
	}
	if(!_userTerminal.hasKnownTerminal())
		std::cout << "Warning: as no known terminal has been found, chat is disabled" << std::endl;
	else
		initListener();  // creates the new thread which listens for entring chat conenctions
	sf::sleep(SOCKET_TIME_SLEEP);  // wait a quarter second to let the listening thread init the port
	return true;
}

std::string Client::shrinkName(const std::string& name)
{
	return (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
}

void Client::initListener()
{
	_threadLoop.store(true);
	_listenerThread = std::thread(&Client::inputListening, this);
}

void Client::quit()
{
	if(!_isConnected)
		return;
	// tell the server that the player leaves
	sf::Packet packet;
	packet << TransferType::PLAYER_DISCONNECTION;
	_socket.send(packet);
	_inGame.store(false);
	_socket.disconnect();
	_threadLoop.store(false);
	_listenerThread.join();
	_isConnected = false;
}

Client::~Client()
{
	quit();
}

///////// Game management

void Client::startGame()
{
	std::cout << "starting game\n";
	sf::Packet packet;
	packet << TransferType::GAME_REQUEST;
	_socket.send(packet);
	// \TODO: have possibility to stop waiting
	_socket.receive(packet);
	packet >> _inGameOpponentName;
	std::cout << "opponent found: " << _inGameOpponentName << std::endl;
	_inGame = true;
}

sf::TcpSocket& Client::getGameSocket()
{
	if(!_inGame)
		throw std::runtime_error("No socket available: not in game");
	return _inGameSocket;
}

sf::TcpSocket& Client::getGameListeningSocket()
{
	if(!_inGame)
		throw std::runtime_error("No socket available: not in game");
	return _inGameListeningSocket;
}

void Client::waitTillReadyToPlay()
{
	static const sf::Time awaitingDelay(sf::milliseconds(50));  // arbitrary
	while(!_readyToPlay.load())
		sf::sleep(awaitingDelay);
}

/////////// Friends management

const std::vector<std::string>& Client::getFriends()
{
	if(!_isConnected)
		throw NotConnectedException("Unable to send friends");
	updateFriends();
	return _friends;
}

std::vector<std::string> Client::getConnectedFriends()
{
	if(!_isConnected)
		throw NotConnectedException("Unable to send connected friends");
	std::vector<std::string> connectedFriends;
	for(const auto& friendName: _friends)
	{
		// ask the server if player is conencted
		sf::Packet packet;
		packet << TransferType::PLAYER_CHECK_CONNECTION << friendName;
		_socket.send(packet);
		_socket.receive(packet);
		TransferType type;
		packet >> type;
		if(type != TransferType::PLAYER_CHECK_CONNECTION)
			continue;
		bool isPresent;
		packet >> isPresent;
		// add to vector only if friend is present
		if(isPresent)
			connectedFriends.push_back(friendName);
	}
	return connectedFriends;
}

void Client::updateFriends()
{
	if(!_isConnected)
		throw NotConnectedException("Unable to update friends");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_ASKS_FRIENDS;
	_socket.send(packet);
	_socket.receive(packet);
	// std::vector packing has been defined in PacketOverload.hpp
	_friends.clear();
	packet >> _friends;
}

void Client::askNewFriend(const std::string& name)
{
	if(!_isConnected)
		throw NotConnectedException("Unable to ask a new friend");
	// Cannot be friend with yourself
	if(name == _name)
		throw std::runtime_error("Can't be friend with yourself");
	// Don't ask a friend to become your friend
	if(isFriend(name))
		throw std::runtime_error(name + "is already your friend");
	sf::Packet packet;
	packet << TransferType::PLAYER_NEW_FRIEND << name;
	_socket.send(packet);
	// server acknowledges with PLAYER_NEW_FRIEND if request was correctly made and by NOT_EXISTING_FRIEND otherwise
	_socket.receive(packet);
	TransferType type;
	packet >> type;
	if(type == TransferType::PLAYER_NEW_FRIEND)
		_friendsRequests.push_back(name);
}

bool Client::getIncomingFriendshipRequests(std::vector<std::string>& incomingRequests)
{
	sf::Packet packet;
	packet << TransferType::PLAYER_GETTING_FRIEND_REQUESTS;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::PLAYER_GETTING_FRIEND_REQUESTS)
		return false;
	packet >> incomingRequests;
	return true;
}

bool Client::updateFriendshipRequests(std::vector<std::string>& acceptedSentRequests,
	std::vector<std::string>& refusedSentRequests)
{
	sf::Packet packet;
	packet << TransferType::PLAYER_GETTING_FRIEND_REQUESTS_STATE;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::PLAYER_GETTING_FRIEND_REQUESTS_STATE)
		return false;
	packet >> acceptedSentRequests >> refusedSentRequests;
	for(const auto& accpeted: acceptedSentRequests)
		_friends.push_back(accpeted);
	return true;
}

bool Client::isFriend(const std::string& name) const
{
	return std::find(_friends.cbegin(), _friends.cend(), name) != _friends.cend();
}

void Client::removeFriend(const std::string& name)
{
	if(!_isConnected)
		throw NotConnectedException("Unable to remove friend");
	if(!isFriend(name))
		throw std::runtime_error(name + "is not a friend of yours");
	sf::Packet packet;
	// send that the user remove name from its friend list
	packet << TransferType::PLAYER_REMOVE_FRIEND;
	packet << name;
	_socket.send(packet);
}

void Client::acceptFriendshipRequest(const std::string& name, bool accept)
{
	sf::Packet packet;
	packet << TransferType::PLAYER_RESPONSE_FRIEND_REQUEST << name << accept;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType status;
	packet >> status;
	if(status == TransferType::PLAYER_RESPONSE_FRIEND_REQUEST && accept)
		_friends.push_back(name);
}

void Client::startConversation(const std::string& playerName) const
{
	// rest assured the client is connected to a server before trying to access it
	if(!_isConnected)
		throw NotConnectedException("Not connected");
	else if(!_userTerminal.hasKnownTerminal())
		throw std::runtime_error("No known terminal");
	else if(playerName == _name)
		throw std::runtime_error("Chatting with yourself is not allowed");
	else if(!isFriend(playerName))
		throw std::runtime_error("You are only allowed to chat with your friends");
	std::string cmd;
	cmd = _userTerminal.startProgram(
		"WizardPoker_chat",
		{
			"caller",  // parameter 1 is caller/callee
			_serverAddress.toString(),  // parameter 2 is the address to connect to
			std::to_string(_serverPort),  // parameter 3 is the port to connect to
			_name,  // parameter 4 is caller's name
			playerName, // parameter 5 is callee's name
			// there is not more parameters!
		});
	system(cmd.c_str());
}

////////////// Listening thread

// function called by a new thread only
void Client::inputListening()
{
	sf::TcpListener chatListener;
	//~ use of a selector to be non-blocking. There may be a better idea
	sf::SocketSelector selector;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cerr << "Unable to listen to arriving chat connections!" << std::endl;
		_chatListenerPort = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		_chatListenerPort = chatListener.getLocalPort();
	std::cout << "waiting for connections on port " << _chatListenerPort << std::endl;
	selector.add(chatListener);
	while(_threadLoop.load())
	{
		// set waiting to 0.1 second so that the loop variable is checked frequently enough
		if(!selector.wait(SOCKET_TIME_SLEEP))
			continue;
		sf::TcpSocket socket;
		if(chatListener.accept(socket) == sf::Socket::Done)
		{
			sf::Packet packet;
			socket.receive(packet);
			TransferType type;
			packet >> type;
			if(type == TransferType::CHAT_PLAYER_IP)
				startChat(packet);
			else if(type == TransferType::NEW_GAME_SERVER_CONNECTION)
				initInGameConnection(packet);
			else
				std::cerr << "Unknown type of message\n";
			std::cin.ignore();
		}
	}
}

void Client::startChat(sf::Packet& transmission)
{
	sf::Uint32 address;
	sf::Uint16 port;
	std::string otherName, selfName;
	transmission >> address >> port >> selfName >> otherName;
	std::string cmd;
	cmd = _userTerminal.startProgram(
		"WizardPoker_chat",
		{
			"callee",
			std::to_string(address),
			std::to_string(port),
			selfName,
			otherName
		});
	system(cmd.c_str());
}

void Client::initInGameConnection(sf::Packet& transmission)
{
	sf::Uint16 serverListeningPort;
	transmission >> serverListeningPort;
	_inGameSocket.connect(_socket.getRemoteAddress(), serverListeningPort);
	_inGameListeningSocket.connect(_socket.getRemoteAddress(), serverListeningPort);
	_readyToPlay.store(true);
}

