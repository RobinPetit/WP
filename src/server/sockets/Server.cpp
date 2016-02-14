// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Sleep.hpp>
// WizardPoker headers
#include <server/Server.hpp>
#include <common/constants.hpp>
#include <server/ErrorCode.hpp>
#include <common/sockets/TransferType.hpp>
// std-C++ headers
#include <iostream>
#include <algorithm>

const std::string quitPrompt = ":QUIT";

// static functions prototypes
static void waitQuit(std::atomic_bool *done, std::atomic_bool *running);

Server::Server():
	_clients(),
	_listener(),
	_socketSelector(),
	_done(false),
	_threadRunning(false),
	_quitThread()
{}

int Server::start(const unsigned short listenerPort)
{
	if(_listener.listen(listenerPort) != sf::Socket::Done)
		return UNABLE_TO_LISTEN;
	_quitThread = std::thread(waitQuit, &_done, &_threadRunning);
	_threadRunning.store(true);
	sf::sleep(SOCKET_TIME_SLEEP);
	_socketSelector.add(_listener);
	while(!_done.load())
	{
		// if no socket is ready, wait again
		if(!_socketSelector.wait(sf::milliseconds(50)))
			continue;
		// if listener is ready, then a new connection is incoming
		if(_socketSelector.isReady(_listener))
			takeConnection();
		else  // one of the client sockets has received something
			receiveData();
	}
	return SUCCESS;
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
	{
		std::cerr << "Unable to find which socket is ready" << std::endl;
		return;
	}

	sf::Packet packet;
	sf::TcpSocket& client(*(it->second.socket));
	sf::Socket::Status receivalStatus = client.receive(packet);
	if(receivalStatus == sf::Socket::Done)
	{
		TransferType type;
		packet >> type;
		switch(type)
		{
		case TransferType::PLAYER_DISCONNECTION:
			std::cout << "Player " << it->first << " quits the game!" << std::endl;
			removeClient(it);
			break;
		case TransferType::PLAYER_CHECK_CONNECTION:
			checkPresence(it, packet);
		default:
			std::cerr << "Error: unknown code " << static_cast<sf::Uint32>(type) << std::endl;
			break;
		}
	}
	else if(receivalStatus == sf::Socket::Disconnected)
	{
		std::cerr << "Connection with player " << it->first << " is lost: forced disconnection from server" << std::endl;
		removeClient(it);
	}
	else
		std::cerr << "data not well received" << std::endl;
}

void Server::removeClient(const _iterator& it)
{
	// remove from the selector so it won't receive data anymore
	_socketSelector.remove(*(it->second.socket));
	// free the allocated socket
	delete it->second.socket;
	// remove from the map
	_clients.erase(it);
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

void Server::quit()
{
	// in case the method is called even though server has not been manually ended
	_done.store(true);
	_quitThread.join();
	_threadRunning.store(false);
	for(auto it = _clients.begin(); it != _clients.end(); ++it)
	{
		_socketSelector.remove(*(it->second.socket));
		delete it->second.socket;
	}
	_clients.clear();
}

void Server::checkPresence(const _iterator& it, sf::Packet& transmission)
{
	sf::Packet packet;
	std::string nameToCheck;
	transmission >> nameToCheck;
	packet << TransferType::PLAYER_CHECK_CONNECTION << (_clients.find(nameToCheck) != _clients.end());
	it->second.socket->send(packet);
}

static void waitQuit(std::atomic_bool *done, std::atomic_bool *running)
{
	std::cout << "Type '" << quitPrompt << "' to end the server" << std::endl;
	std::string input;
	while(!done->load())
	{
		std::cin >> input;
		if(input == quitPrompt)
			done->store(true);
	}
	running->store(false);
	std::cout << "ending server..." << std::endl;
}
