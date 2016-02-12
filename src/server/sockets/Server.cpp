// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include <server/Server.hpp>
#include <common/constants.hpp>
#include <server/ErrorCode.hpp>
#include <common/sockets/TransferType.hpp>
// std-C++ headers
#include <iostream>
#include <algorithm>

Server::Server():
	_clients(),
	_listener(),
	_socketSelector(),
	_done(false)
{}

int Server::start(const unsigned short listenerPort)
{
	if(_listener.listen(listenerPort) != sf::Socket::Done)
		return UNABLE_TO_LISTEN;
	_socketSelector.add(_listener);
	while(!_done)
	{
		// if no socket is ready, wait again
		if(!_socketSelector.wait())
			continue;
		// if listener is ready, then a new connection is incoming
		if(_socketSelector.isReady(_listener))
			takeConnection();
		else  // one of the client sockets has received something
			receiveData();
	}
	return SERVER_OK;
}

void Server::takeConnection()
{
	std::string playerName;
	sf::Uint16 clientPort;
	sf::TcpSocket *newClient = new sf::TcpSocket();
	// if listener can't accept correctly, free the allocated socket
	if(_listener.accept(*newClient) != sf::Socket::Done)
	{
		delete newClient;
		return;
	}
	// receive username
	sf::Packet packet;
	newClient->receive(packet);
	TransferType type;
	packet >> type;
	if(type == TransferType::GAME_CONNECTION)
	{
		packet >> playerName >> clientPort;
		/// \TODO receive the password and check the connection right here
		std::cout << "new player connected: " << playerName << std::endl;
		// add the new socket to the clients
		_clients[playerName] = {newClient, clientPort};
		// and add this client to the selector so that its receivals are handled properly
		_socketSelector.add(*newClient);
	}
	else if(type == TransferType::CHAT_PLAYER_IP)
		handleChatRequest(packet, *newClient);
	else
		std::cout << "Error: wrong code!" << std::endl;
}

void Server::receiveData()
{
        std::cout << "Data received\n";
	// first find which socket it is
	auto it = std::find_if(_clients.begin(), _clients.end(), [this](const auto& pair)
	{
		return _socketSelector.isReady(*(pair.second.socket));
	});
	if(it == _clients.end())
		return;

	sf::Packet packet;
	sf::TcpSocket& client(*(it->second.socket));
	if(client.receive(packet) == sf::Socket::Done)
	{
		sf::Packet packet;
		if(client.receive(packet) == sf::Socket::Done)
		{
			TransferType type;
			packet >> type;
			switch(type)
			{
			default:
				std::cerr << "Error: unknown code " << static_cast<int>(type) << std::endl;
				break;
			}
		}
	}
}

void Server::handleChatRequest(sf::Packet& packet, sf::TcpSocket& client)
{
	sf::Packet responseToCaller;
	responseToCaller << TransferType::CHAT_PLAYER_IP;
	std::string calleeName;
	std::string callerName;
	sf::Uint16 callerPort;
	packet >> callerName >> calleeName >> callerPort;
	std::cout << "start request\n";
	// first of all, verify that player exist
	// if it does, send his IP
	auto callee = _clients.find(calleeName);
	if(callee == _clients.end())
	{
		std::cout << "player does not exist!\n";
		responseToCaller << static_cast<sf::Uint32>(0);
	}
	else
	{
		responseToCaller << _clients[calleeName].socket->getRemoteAddress().toInteger();
		sf::Packet packetToCalle;
		sf::TcpSocket toCallee;
		std::cout << "address is " << _clients[calleeName].socket->getRemoteAddress() << " and port is " << _clients[calleeName].listeningPort << std::endl;
		if(toCallee.connect(_clients[calleeName].socket->getRemoteAddress(), _clients[calleeName].listeningPort) != sf::Socket::Done)
			std::cerr << "Unable to connect to calle (" << calleeName << ")\n";
		else
		{
			std::cout << "connected\n";
			// as the listening port has a delay, set communications as blokcing
			toCallee.setBlocking(true);
			packetToCalle << client.getRemoteAddress().toInteger() << callerPort << calleeName << callerName;
			std::cout << "sending to callee\n";
			if(toCallee.send(packetToCalle) != sf::Socket::Done)
				std::cerr << "unable to send to calle\n";
			else
				std::cout << "sent to callee (" << calleeName << ")\n";
		}
	}
	client.send(responseToCaller);
}

