// SFML headers
#include <SFML/System/Sleep.hpp>
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/constants.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/NotConnectedException.hpp"
#include "common/sockets/PacketOverload.hpp"
// std-C++ headers
#include <iostream>
#include <thread>
#include <cstdlib>
#include <algorithm>

extern void chatListening(sf::Uint16 *port, const std::atomic_bool *loop, Terminal terminal);

Client::Client():
	_socket(),
	_currentConversations(),
	_isConnected(false),
	_chatListenerPort(0),
	_threadLoop(0),
	_serverAddress(),
	_serverPort(0),
	_userTerminal()
{

}

bool Client::connectToServer(const std::string& name, const sf::IpAddress& address, sf::Uint16 port)
{
	// if client is already connected to a server, do not try to re-connect it
	if(_isConnected)
		return false;
	// forces the name to not be larger than MAX_NAME_LENGTH
	_name = (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
	_serverAddress = address;
	_serverPort = port;
	if(!_userTerminal.hasKnownTerminal())
		std::cout << "Warning: as no known terminal has been found, chat is disabled" << std::endl;
	else
		initListener();  // creates the new thread which listens for entring chat conenctions
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
		return false;
	sf::sleep(SOCKET_TIME_SLEEP);  // wait a quarter second to let the listening thread init the port
	sf::Packet packet;
	packet << TransferType::GAME_CONNECTION  // precise
	       << _name  // do not forget the '\0' character
	       << static_cast<sf::Uint16>(_chatListenerPort);
	if(_socket.send(packet) != sf::Socket::Done)
		return false;
	_isConnected = true;
	updateFriends();
	return true;
}

const std::vector<std::string>& Client::getFriends()
{
	if(!_isConnected)
		throw NotConnectedException("Unable to send friends");
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
	packet >> _friends;
}

bool Client::askNewFriend(const std::string& name)
{
	if(!_isConnected)
		throw NotConnectedException("Unable to ask a new friend");
	// Cannot be friend with yourself
	if(name == _name)
		return false;
	// Don't ask a friend to become your friend
	if(isFriend(name))
		return false;
	sf::Packet packet;
	packet << TransferType::PLAYER_NEW_FRIEND << name;
	_socket.send(packet);
	// server acknowledges with PLAYER_NEW_FRIEND if request was correctly made and by NOT_EXISTING_FRIEND otherwise
	_socket.receive(packet);
	TransferType type;
	packet >> type;
	if(type == TransferType::PLAYER_NEW_FRIEND)
		_friendsRequests.push_back(name);
	return true;
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

bool Client::isFriend(const std::string& name)
{
	return std::find(_friends.cbegin(), _friends.cend(), name) != _friends.cend();
}

bool Client::removeFriend(const std::string& name)
{
	if(!isFriend(name))
		return false;
	_friends.erase(std::find(_friends.cbegin(), _friends.cend(), name));
	return true;
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

bool Client::startConversation(const std::string& playerName)
{
	// rest assured the client is connected to a server before trying to access it
	if(!_isConnected || !_userTerminal.hasKnownTerminal() || playerName == _name || !isFriend(playerName))
		return false;
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
	return true;
}

void Client::initListener()
{
	_threadLoop.store(true);
	_listenerThread = std::thread(chatListening, &_chatListenerPort, &_threadLoop, _userTerminal);
}

void Client::quit()
{
	// tell the server that the player leaves
	sf::Packet packet;
	packet << TransferType::PLAYER_DISCONNECTION;
	_socket.send(packet);
	_threadLoop.store(false);
	_listenerThread.join();
}

