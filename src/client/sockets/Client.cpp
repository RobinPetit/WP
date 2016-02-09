#include <client/sockets/Client.hpp>
#include <common/constants.hpp>
#include <common/sockets/TransferType.hpp>
#include <iostream>

Client::Client(const std::string& name):
	_socket(),
	_currentConversations(),
	_isConnected(false)
{
	// forces the name to not be larger than MAX_NAME_LENGTH
	_name = (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
}

bool Client::connectToServer(const sf::IpAddress& address, int port)
{
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
		return false;
	// do not forget the '\0' character
	return _isConnected = (_socket.send(_name.c_str(), _name.size()+1) == sf::Socket::Done);
}

bool Client::startConversation(const std::string& playerName)
{
	if(!_isConnected)
		return false;
	sf::Packet sentPacket;
    sentPacket << TransferType::CHAT_PLAYER_IP << playerName;
    if(_socket.send(sentPacket) != sf::Socket::Done)
		return false;
	sf::Packet receivedPacket;
	if(_socket.receive(receivedPacket) != sf::Socket::Done)
		return false;
	TransferType type;
	receivedPacket >> type;
	if(type != TransferType::CHAT_PLAYER_IP)
		return false;
	sf::Uint32 playerIP;
	receivedPacket >> playerIP;
	return true;
}

