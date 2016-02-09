#include <server/Server.hpp>
#include <common/constants.hpp>
#include <server/ErrorCode.hpp>

#include <iostream>
#include <algorithm>

Server::Server():
	_clients(),
	_listener(),
	_socketSelector(),
	_done(false)
{}

bool Server::start(const unsigned short listenerPort)
{
	if(_listener.listen(listenerPort) != sf::Socket::Done)
		return UNABLE_TO_LISTEN;
	_socketSelector.add(_listener);
	sf::TcpSocket *newClient(nullptr);
	while(!_done)
	{
		/* if no socket is ready, wait again */
		if(!_socketSelector.wait())
			continue;
		/* if listener is ready, then a new connection is incoming */
		if(_socketSelector.isReady(_listener))
			takeConnection();
		else /* one of the client sockets has received something */
			receiveData();
	}
	return SERVER_OK;
}

void Server::takeConnection()
{
	char playerName[MAX_NAME_LENGTH];
	size_t receivedBytes;
	sf::TcpSocket *newClient = new sf::TcpSocket();
	/* if listener can't accept correctly, free the allocated socket */
	if(_listener.accept(*newClient) != sf::Socket::Done)
	{
		delete newClient;
		return;
	}
	/* receive username */
	newClient->receive(playerName, MAX_NAME_LENGTH, receivedBytes);
	/** \TODO receive the password and check the connection right here **/
	std::cout << "new player connected: " << playerName << std::endl;
	/* add the new socket to the clients */
	_clients[playerName] = newClient;
	/* and add this client to the selector so that its receivals are handled properly */
	_socketSelector.add(*newClient);
}

void Server::receiveData()
{
	/* first find which socket it is */
	auto it = std::find_if(_clients.begin(), _clients.end(), [this](const auto& pair)
	{
		return _socketSelector.isReady(*pair.second);
	});

	if(it != _clients.end())
	{
		sf::Packet packet;
		sf::TcpSocket& client(*it->second);
		if(client.receive(packet) == sf::Socket::Done)
		{
			std::cout << "Data is received" << std::endl;
			/* handle received data */
		}
	}
}

