// SFML headers
#include <SFML/System/Sleep.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/ErrorCode.hpp"
#include "client/NonBlockingInput.hpp"
#include "common/constants.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/NotConnectedException.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "common/Deck.hpp"
// std-C++ headers
#include <iostream>
#include <thread>
#include <algorithm>
#include <atomic>
#include <string>

Client::Client():
	_socket(),
	_chatListenerPort(0),
	_isConnected(false),
	_name(),
	_listenerThread(),
	_serverAddress(),
	_serverPort(0),
	_threadLoop(false),
	_userTerminal(),
	_currentConversations(),
	_inGame(false),
	_inGameSocket(),
	_inGameListeningSocket(),
	_inGameOpponentName(),
	_readyToPlay(false)
{

}

void Client::connectToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port)
{
	initServer(name, address, port);
	sendConnectionToken(password);
}

void Client::registerToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port)
{
	std::string shrinkedName = shrinkName(name);
	// The local socket used only to register, it does not need to be keeped as attribute
	sf::TcpSocket socket;
	// if connection does not work, don't go further
	if(socket.connect(address, port) != sf::Socket::Done)
		throw std::runtime_error("unable to connect to server on port " + std::to_string(port) + ".");
	sendRegisteringToken(shrinkedName, password, socket);
}

void Client::sendConnectionToken(const std::string& password)
{
	sf::Packet packet;
	packet << TransferType::GAME_CONNECTION
	       << _name
	       << password
	       << static_cast<sf::Uint16>(_chatListenerPort);
	if(_socket.send(packet) != sf::Socket::Done)
		throw std::runtime_error("failed to send connection packet.");

	// Receive the server response
	_socket.receive(packet);
	TransferType response;
	packet >> response;
	switch(response)
	{
	case TransferType::GAME_ALREADY_CONNECTED:
		throw std::runtime_error("you are already connected!");

	case TransferType::GAME_WRONG_IDENTIFIERS:
		throw std::runtime_error("invalid username or password.");

	case TransferType::ACKNOWLEDGE:
		_isConnected = true;
		updateFriends();
		break;

	default:
		throw std::runtime_error("unidentified server response.");
	}
}
void Client::sendRegisteringToken(const std::string& name, const std::string& password, sf::TcpSocket& socket)
{
	sf::Packet packet;
	packet << TransferType::GAME_REGISTERING << name << password;
	if(socket.send(packet) != sf::Socket::Done)
		throw std::runtime_error("sending packet failed.");

	// Receive the server response
	socket.receive(packet);
	TransferType response;
	packet >> response;
	switch(response)
	{
	case TransferType::GAME_USERNAME_NOT_AVAILABLE:
		//TODO throw an exception rather than cout
		throw std::runtime_error("the username " + name + " is not available.");

	case TransferType::GAME_FAILED_TO_REGISTER:
		throw std::runtime_error("the server failed to register your account.");

	case TransferType::ACKNOWLEDGE:
		return;

	default:
		throw std::runtime_error("unidentified server response.");
	}
}

void Client::initServer(const std::string& name, const sf::IpAddress& address, sf::Uint16 port)
{
	// if client is already connected to a server, do not try to re-connect it
	if(_isConnected)
		throw std::runtime_error("already connected.");
	_name = shrinkName(name);
	_serverAddress = address;
	_serverPort = port;
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
		throw std::runtime_error("unable to connect to server on port " + std::to_string(port) + ".");
	if(!_userTerminal.hasKnownTerminal())
		std::cout << "Warning: as no known terminal has been found, chat is disabled" << std::endl;
	else
		initListener();  // creates the new thread which listens for entring chat conenctions
	sf::sleep(SOCKET_TIME_SLEEP);  // wait a quarter second to let the listening thread init the port
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
	// If the connection is well established with the server
	if(_isConnected)
	{
		// tell the server that the player leaves
		sf::Packet packet;
		packet << TransferType::PLAYER_DISCONNECTION;
		_socket.send(packet);
	}
	// If a connection failed, the socket is connected but the server
	// no longer listen to it, so quit must be called even if _isConnected is false
	_socket.disconnect();
	// internal part
	_inGame.store(false);
	_threadLoop.store(false);
	if(_listenerThread.joinable())
		_listenerThread.join();
	_isConnected = false;
}

Client::~Client()
{
	quit();
}

///////// Game management

bool Client::startGame()
{
	static const std::string cancelWaitingString{"q"};
	NonBlockingInput input;
	std::cout << "Entering the lobby. Type '" << cancelWaitingString << "' to leave.\n";
	// send a request for the server to place the client in its internal lobby
	sf::Packet packet;
	packet << TransferType::GAME_REQUEST;
	_socket.send(packet);
	// use a selector to
	sf::SocketSelector selector;
	selector.add(_socket);
	bool loop{true};
	// wait for a server response (opponent found) or a user entry to leave
	while(loop)
	{
		// if the server found an opponent
		if(selector.wait(sf::milliseconds(50)))
		{
			loop = false;
			_socket.receive(packet);
			packet >> _inGameOpponentName;
			std::cout << "opponent found: " << _inGameOpponentName << std::endl;
			_inGame = true;
		}
		// if the player typed something and it is the right string
		else if(input.waitForData(0.05) && input.receiveStdinData() == cancelWaitingString)
		{
			loop =  _inGame = false;
			// send a request to leave the lobby
			packet.clear();
			packet << TransferType::GAME_CANCEL_REQUEST;
			_socket.send(packet);
		}
	}
	return _inGame;
}

sf::TcpSocket& Client::getGameSocket()
{
	if(!_inGame)
		throw std::runtime_error("no socket available: not in game.");
	return _inGameSocket;
}

sf::TcpSocket& Client::getGameListeningSocket()
{
	if(!_inGame)
		throw std::runtime_error("no socket available: not in game.");
	return _inGameListeningSocket;
}

void Client::waitTillReadyToPlay()
{
	static const sf::Time awaitingDelay(sf::milliseconds(50));  // arbitrary
	while(!_readyToPlay.load())
		sf::sleep(awaitingDelay);
}

const Terminal& Client::getTerminal()
{
	return _userTerminal;
}

/////////////////// Friends management

const FriendsList& Client::getFriends()
{
	if(!_isConnected)
		throw NotConnectedException("unable to send friends.");
	updateFriends();
	return _friends;
}

FriendsList Client::getConnectedFriends()
{
	if(!_isConnected)
		throw NotConnectedException("unable to send connected friends.");
	updateFriends();
	FriendsList connectedFriends;
	for(const auto& friendUser: _friends)
	{
		// ask the server if player is conencted
		sf::Packet packet;
		packet << TransferType::PLAYER_CHECK_CONNECTION << friendUser.name;
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
			connectedFriends.push_back(friendUser);
	}
	return connectedFriends;
}

const FriendsList& Client::getFriendshipRequests()
{
	if(!_isConnected)
		throw NotConnectedException("unable to send friendship requests.");
	updateFriendshipRequests();
	return _friendshipRequests;
}

void Client::updateFriends()
{
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_ASKS_FRIENDS;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to get friends list.");
	// FriendsList packing has been defined in PacketOverload.hpp
	_friends.clear();
	packet >> _friends;
}

void Client::updateFriendshipRequests()
{
	sf::Packet packet;
	// send that requests list is asked
	packet << TransferType::PLAYER_GETTING_FRIEND_REQUESTS;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to get friendship requests list.");
	_friendshipRequests.clear();
	packet >> _friendshipRequests;
}

void Client::sendFriendshipRequest(const std::string& name)
{
	if(!_isConnected)
		throw NotConnectedException("unable to ask a new friend.");
	// Cannot be friend with yourself
	if(name == _name)
		throw std::runtime_error("can't be friend with yourself.");
	// Don't ask a friend to become your friend
	if(isFriend(name))
		throw std::runtime_error(name + "is already your friend.");
	sf::Packet packet;
	packet << TransferType::PLAYER_NEW_FRIEND << name;
	_socket.send(packet);
	// server acknowledges with ACKNOWLEDGE if request was correctly made and by NOT_EXISTING_FRIEND otherwise
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("failed to send a request to " + name + ".");
}


bool Client::isFriend(const std::string& name) const
{
	return std::find_if(_friends.cbegin(), _friends.cend(), [&name](const Friend& friendUser)
	{
		return name == friendUser.name;
	}) != _friends.cend();
}

void Client::removeFriend(const std::string& name)
{
	if(!_isConnected)
		throw NotConnectedException("unable to remove friend.");
	if(!isFriend(name))
		throw std::runtime_error(name + "is not a friend of yours.");
	sf::Packet packet;
	// send that the user remove name from its friend list
	packet << TransferType::PLAYER_REMOVE_FRIEND;
	packet << name;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("failed to remove " + name + " from your friend list.");
}

void Client::acceptFriendshipRequest(const std::string& name, bool accept)
{
	sf::Packet packet;
	packet << TransferType::PLAYER_RESPONSE_FRIEND_REQUEST << name << accept;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader == TransferType::NOT_EXISTING_FRIEND)
		throw std::runtime_error("it seems that " + name + " does not exists.");
	else if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("failed send response from friendship request to " + name + ".");
}

void Client::startConversation(const std::string& playerName) const
{
	// rest assured the client is connected to a server before trying to access it
	if(!_isConnected)
		throw NotConnectedException("not connected.");
	else if(!_userTerminal.hasKnownTerminal())
		throw std::runtime_error("no known terminal.");
	else if(playerName == _name)
		throw std::runtime_error("chatting with yourself is not allowed.");
	else if(!isFriend(playerName))
		throw std::runtime_error("you are only allowed to chat with your friends.");
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

//////////////// Cards managment

std::vector<Deck> Client::getDecks()
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_ASKS_DECKS_LIST;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to get the decks list.");
	std::vector<Deck> deckList;
	packet >> deckList;
	return deckList;
}

void Client::handleDeckEditing(const Deck& editedDeck)
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_EDIT_DECK << editedDeck;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to send deck editing to the server.");
}

void Client::handleDeckCreation(const Deck& createdDeck)
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_CREATE_DECK << createdDeck;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to send deck editing to the server.");
}

void Client::handleDeckDeletion(const std::string& deletedDeckName)
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_DELETE_DECK << deletedDeckName;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to send deck deleting to the server.");
}

CardsCollection Client::getCardsCollection()
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_ASKS_CARDS_COLLECTION;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to get the card collection.");
	CardsCollection cardCollection;
	packet >> cardCollection;
	return cardCollection;
}

////////////////// Others

Ladder Client::getLadder()
{
	if(!_isConnected)
		throw NotConnectedException("unable to get the decks list.");
	sf::Packet packet;
	// send that friends list is asked
	packet << TransferType::PLAYER_ASKS_LADDER;
	_socket.send(packet);
	_socket.receive(packet);
	TransferType responseHeader;
	packet >> responseHeader;
	if(responseHeader != TransferType::ACKNOWLEDGE)
		throw std::runtime_error("unable to get the ladder.");
	Ladder ladder;
	packet >> ladder;
	return ladder;
}
