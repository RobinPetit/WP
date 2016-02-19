// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Sleep.hpp>
// WizardPoker headers
#include "server/Server.hpp"
#include "common/constants.hpp"
#include "server/ErrorCode.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
// std-C++ headers
#include <iostream>
#include <algorithm>

Server::Server():
	_clients(),
	_listener(),
	_socketSelector(),
	_done(false),
	_threadRunning(false),
	_quitThread(),
	_waitingPlayer(),
	_isAPlayerWaiting(false),
	_quitPrompt(":QUIT"),
	_last_id(0)
{

}

int Server::start(const sf::Uint16 listenerPort)
{
	if(_listener.listen(listenerPort) != sf::Socket::Done)
		return UNABLE_TO_LISTEN;
	_quitThread = std::thread(&Server::waitQuit, this);
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
		_clients[playerName] = {newClient, clientPort, {}, {}, {}, {}, _last_id++};
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
		// Friendship management
		case TransferType::PLAYER_CHECK_CONNECTION:
			checkPresence(it, packet);
			break;
		case TransferType::PLAYER_ASKS_FRIENDS:
			sendFriends(it);
			break;
		case TransferType::PLAYER_NEW_FRIEND:
			handleFriendshipRequest(it, packet);
			break;
		case TransferType::PLAYER_REMOVE_FRIEND:
			handleRemoveFriend(it, packet);
			break;
		case TransferType::PLAYER_RESPONSE_FRIEND_REQUEST:
			handleFriendshipRequestResponse(it, packet);
			break;
		case TransferType::PLAYER_GETTING_FRIEND_REQUESTS_STATE:
			sendFriendshipRequestsState(it);
			break;
		case TransferType::PLAYER_GETTING_FRIEND_REQUESTS:
			sendFriendshipRequests(it);
			break;
		// Game management
		case TransferType::GAME_REQUEST:
			findOpponent(it);
			break;
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

void Server::checkPresence(const _iterator& it, sf::Packet& transmission)
{
	sf::Packet packet;
	std::string nameToCheck;
	transmission >> nameToCheck;
	packet << TransferType::PLAYER_CHECK_CONNECTION << (_clients.find(nameToCheck) != _clients.end());
	it->second.socket->send(packet);
}

void Server::quit()
{
	// End game threads
	for(auto& gameThread: _runningGames)
	{
		gameThread->interruptGame();
		gameThread->join();
	}
	_runningGames.clear();
	// in case the method is called even though server has not been manually ended
	_done.store(true);
	_quitThread.join();
	_threadRunning.store(false);
	for(auto& client : _clients)
	{
		_socketSelector.remove(*(client.second.socket));
		delete client.second.socket;
	}
	_clients.clear();
}

Server::~Server()
{
	quit();
}

void Server::waitQuit()
{
	std::cout << "Type '" << _quitPrompt << "' to end the server" << std::endl;
	std::string input;
	while(!_done.load())
	{
		std::cin >> input;
		if(input == _quitPrompt)
			_done.store(true);
	}
	_threadRunning.store(false);
	std::cout << "ending server..." << std::endl;
}

// Game management

void Server::findOpponent(const _iterator& it)
{
	if(!_isAPlayerWaiting)
	{
		_isAPlayerWaiting = true;
		_waitingPlayer = it->first;
	}
	else
	{
		const auto& waitingPlayer = _clients.find(_waitingPlayer);
		// if waiting player is not present anymore
		if(waitingPlayer == _clients.end())
			_waitingPlayer = it->first;
		else
		{
			sf::Packet toFirst, toSecond;
			toFirst << it->first;
			toSecond << _waitingPlayer;
			it->second.socket->send(toSecond);
			waitingPlayer->second.socket->send(toFirst);
			_isAPlayerWaiting = false;
			createGame(waitingPlayer->second.id, it->second.id);
		}
	}
}

void Server::startGame(std::size_t len)
{
	std::cout << "Game " << len << " is starting\n";
}

void Server::createGame(unsigned ID1, unsigned ID2)
{
	GameThread *g = new GameThread(ID1, ID2, &Server::startGame, this, _runningGames.size());
	_runningGames.push_back(g);
}

// Friends management

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

void Server::handleFriendshipRequest(const _iterator& it, sf::Packet& transmission)
{
	sf::Packet response;
	std::string name;
	transmission >> name;
	/// \TODO check if name is in the database so that is the requested player exists but is not connected,
	/// the request in stored somewhere
	//~ Right now, the code only works if both players are conencted simultaneously
	const _iterator& aimedPlayer = _clients.find(name);
	if(aimedPlayer == _clients.end())
		response << TransferType::NOT_EXISTING_FRIEND;
	else
	{
		it->second.friendshipRequests.push_back(name);
		aimedPlayer->second.externalRequests.push_back(it->first);
		response << TransferType::PLAYER_NEW_FRIEND;
	}
	it->second.socket->send(response);
}

void Server::handleFriendshipRequestResponse(const _iterator& it, sf::Packet& transmission)
{
	bool accepted;
	std::string name;
	transmission >> name >> accepted;
	const auto& asker = _clients.find(name);
	// if request corresponds to a non-connected player, don't go further
	if(asker == _clients.end())
	{
		sf::Packet response;
		response << TransferType::NOT_EXISTING_FRIEND;
		it->second.socket->send(response);
		return;
	}
	// remove requests for both players
	auto& extRequests = it->second.externalRequests;
	extRequests.erase(std::find(extRequests.begin(), extRequests.end(), name));
	auto& friendRequests = asker->second.friendshipRequests;
	friendRequests.erase(std::find(friendRequests.begin(), friendRequests.end(), it->first));
	// add the accepter's name in the correct list
	(accepted ? asker->second.acceptedRequests : asker->second.refusedRequests).push_back(it->first);
	// acknowledge to client
	sf::Packet response;
	response << TransferType::PLAYER_RESPONSE_FRIEND_REQUEST;
	it->second.socket->send(response);
}

void Server::sendFriendshipRequests(const _iterator& it)
{
	sf::Packet response;
	response << TransferType::PLAYER_GETTING_FRIEND_REQUESTS << it->second.externalRequests;
	it->second.socket->send(response);
}

void Server::sendFriendshipRequestsState(const _iterator& it)
{
	sf::Packet response;
	response << TransferType::PLAYER_GETTING_FRIEND_REQUESTS_STATE
	         << it->second.acceptedRequests
	         << it->second.refusedRequests;
	it->second.socket->send(response);
	// Remove the temporary data that are the accepted and refused friendship lists
	auto& accepted = it->second.acceptedRequests;
	auto& refused = it->second.refusedRequests;
	accepted.erase(accepted.cbegin(), accepted.cend());
	refused.erase(refused.cbegin(), refused.cend());
}

void Server::sendFriends(const _iterator& it)
{
	std::vector<std::string> friends;
	/// \TODO use database to find the friends
	sf::Packet packet;
	packet << friends;
	it->second.socket->send(packet);
}

void Server::handleRemoveFriend(const _iterator& it, sf::Packet& transmission)
{
	std::string removedFriend;
	transmission >> removedFriend;
	// TODO update database, remove removedFriend from the friend list of it
}

