// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Sleep.hpp>
// WizardPoker headers
#include "server/Server.hpp"
#include "common/constants.hpp"
#include "common/Identifiers.hpp"
#include "server/ErrorCode.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
// std-C++ headers
#include <iostream>
#include <algorithm>

Server::Server():
	_clients(),
	_socketSelector(),
	_done(false),
	_threadRunning(false),
	_quitThread(),
	_waitingPlayer(),
	_isAPlayerWaiting(false),
	_quitPrompt(":QUIT"),
	_database()
{
}

int Server::start(const sf::Uint16 listenerPort)
{
	sf::TcpListener listener;
	if(listener.listen(listenerPort) != sf::Socket::Done)
		return UNABLE_TO_LISTEN;
	_quitThread = std::thread(&Server::waitQuit, this);
	_threadRunning.store(true);
	sf::sleep(SOCKET_TIME_SLEEP);
	_socketSelector.add(listener);
	while(!_done.load())
	{
		// if no socket is ready, wait again
		if(!_socketSelector.wait(sf::milliseconds(50)))
			continue;
		// if listener is ready, then a new connection is incoming
		if(_socketSelector.isReady(listener))
			takeConnection(listener);
		else  // one of the client sockets has received something
			receiveData();
	}
	return SUCCESS;
}

void Server::takeConnection(sf::TcpListener& listener)
{
	std::unique_ptr<sf::TcpSocket> newClient{new sf::TcpSocket()};
	// if listener can't accept correctly, free the allocated socket
	if(listener.accept(*newClient) != sf::Socket::Done)
	{
		std::cout << "Error when trying to accept a new client.\n";
		return;
	}
	// receive username
	sf::Packet packet;
	newClient->receive(packet);
	TransferType type;
	packet >> type;
	if(type == TransferType::CONNECTION)
		connectUser(packet, std::move(newClient));
	else if(type == TransferType::REGISTERING)
		registerUser(packet, std::move(newClient));
	else if(type == TransferType::CHAT_PLAYER_IP)
		handleChatRequest(packet, std::move(newClient));
	else
		std::cout << "Error: wrong code!" << std::endl;
}

void Server::connectUser(sf::Packet& connectionPacket, std::unique_ptr<sf::TcpSocket> client)
{
	std::string playerName, password;
	sf::Uint16 clientPort;
	connectionPacket >> playerName >> password >> clientPort;
	connectionPacket.clear();
	try
	{
		// Check if the user is not already connected
		if(_clients.find(playerName) != _clients.end())
		{
			connectionPacket << TransferType::ALREADY_CONNECTED;
			throw std::runtime_error(playerName + " tried to connect to the server but is already connected.");
		}

		// FIXME : this method call returns always false _database.areIdentifiersValid(playerName, password))
		if(not _database.isRegistered(playerName))
		{
			connectionPacket << TransferType::WRONG_IDENTIFIERS;
			throw std::runtime_error(playerName + " gives wrong identifiers when trying to connect.");
		}
		std::cout << "New player connected: " << playerName << std::endl;
		connectionPacket << TransferType::ACKNOWLEDGE;
		// Send a response,
		client->send(connectionPacket);
		// add this client to the selector so that its receivals are handled properly
		// (be sure that this line is before the next, otherwise we get a segfault),
		_socketSelector.add(*client);
		// ask the database for the ID of the user (may throw, so keep it in
		// a separate line from the insertion in the map),
		const userId id{_database.getUserId(playerName)};
		// and add the new socket to the clients
		_clients[playerName] = {std::move(client), clientPort, id};
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "connectUser error: " << e.what() << "\n";
		// Send a response
		client->send(connectionPacket);
	}
	// The line client->send(connectionPacket); cannot be placed here to factorize it
	// because if everything goes well, the sokect is moved into _clients,
	// so the unique_ptr client become invalid.
}

void Server::registerUser(sf::Packet& registeringPacket, std::unique_ptr<sf::TcpSocket> client)
{
	std::string playerName, password;
	registeringPacket >> playerName >> password;
	registeringPacket.clear();
	try
	{

		if(_database.isRegistered(playerName))
		{
			registeringPacket << TransferType::USERNAME_NOT_AVAILABLE;
			throw std::runtime_error(playerName + " tried to register to the server but the name is not available.");
		}
		_database.registerUser(playerName, password);
		registeringPacket << TransferType::ACKNOWLEDGE;
		std::cout << "New player registered: " << playerName << std::endl;
	}
	catch(const std::runtime_error& e)
	{
		// If Database::registerUser threw an exception, the packet is empty
		if(registeringPacket.getDataSize() == 0)
			registeringPacket << TransferType::FAILED_TO_REGISTER;

		std::cout << "registerUser error: " << e.what() << "\n";
	}
	// Send a response
	client->send(registeringPacket);
}

void Server::receiveData()
{
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
	std::cout << "Data received from " + userToString(it) + "\n";

	if(receivalStatus == sf::Socket::Done)
	{
		TransferType type;
		packet >> type;
		switch(type)
		{
		case TransferType::DISCONNECTION:
			std::cout << "Player " + userToString(it) + " quits the game!" << std::endl;
			removeClient(it);
			break;
		// Friendship management
		case TransferType::CHECK_PRESENCE:
			checkPresence(it, packet);
			break;
		case TransferType::ASK_FRIENDS:
			sendFriends(it);
			break;
		case TransferType::NEW_FRIEND:
			handleFriendshipRequest(it, packet);
			break;
		case TransferType::REMOVE_FRIEND:
			handleRemoveFriend(it, packet);
			break;
		case TransferType::RESPONSE_FRIEND_REQUEST:
			handleFriendshipRequestResponse(it, packet);
			break;
		case TransferType::GET_FRIEND_REQUESTS:
			sendFriendshipRequests(it);
			break;
		// Game management
		case TransferType::GAME_REQUEST:
			findOpponent(it);
			break;
		case TransferType::GAME_CANCEL_REQUEST:
			clearLobby(it);
			break;
		// Cards management
		case TransferType::ASK_DECKS_LIST:
			sendDecks(it);
			break;
		case TransferType::EDIT_DECK:
			handleDeckEditing(it, packet);
			break;
		case TransferType::CREATE_DECK:
			handleDeckCreation(it, packet);
			break;
		case TransferType::DELETE_DECK:
			handleDeckDeletion(it, packet);
			break;
		case TransferType::ASK_CARDS_COLLECTION:
			sendCardsCollection(it);
			break;
		// Others
		case TransferType::ASK_LADDER:
			sendLadder(it);
			break;
		default:
			std::cerr << "Error: unknown code " << static_cast<sf::Uint32>(type) << std::endl;
			break;
		}
	}
	else if(receivalStatus == sf::Socket::Disconnected)
	{
		std::cerr << "Connection with player " + userToString(it) + " is lost: forced disconnection from server.\n";
		removeClient(it);
	}
	else
		std::cerr << "Data not well received.\n";
}

void Server::removeClient(const _iterator& it)
{
	// remove from the selector so it won't receive data anymore
	_socketSelector.remove(*(it->second.socket));
	// remove from the map
	_clients.erase(it);
}

void Server::checkPresence(const _iterator& it, sf::Packet& transmission)
{
	sf::Packet packet;
	std::string nameToCheck;
	transmission >> nameToCheck;
	if(_database.areFriend(it->second.id, _clients[nameToCheck].id))
		packet << TransferType::ACKNOWLEDGE << (_clients.find(nameToCheck) != _clients.end());
	else
		packet << TransferType::FAILURE;
	it->second.socket->send(packet);
}

void Server::quit()
{
	// End game threads
	for(auto& gameThread: _runningGames)
	{
		gameThread->interruptGame();
		// Avoid to throw an exception if not joinable, just in case
		if(gameThread->joinable())
			gameThread->join();
	}
	_runningGames.clear();
	// in case the method is called even though server has not been manually ended
	_done.store(true);
	_quitThread.join();
	_threadRunning.store(false);
	_socketSelector.clear();
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

std::string Server::userToString(const _iterator& it)
{
	return it->first + " (" + it->second.socket->getRemoteAddress().toString() + ")";
}

//////////////// Game management

void Server::findOpponent(const _iterator& it)
{
	std::lock_guard<std::mutex> lockLobby{_lobbyMutex};
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
	// _lobbyMutex is unlocked when lockLobby is destructed
}

void Server::clearLobby(const _iterator& it)
{
	std::lock_guard<std::mutex> lockLobby{_lobbyMutex};
	if(not _isAPlayerWaiting or _waitingPlayer != it->first)
		throw std::runtime_error("Trying to remove another player from lobby; ignored\n");
	_isAPlayerWaiting = false;
	// _lobbyMutex is unlocked when lockLobby is destructed
}

void Server::startGame(std::size_t idx)
{
	// A unique lock also releases the mutex at destruction (just like
	// std::lock_guard) but we can explicitly lock and unlock it, combining
	// the benefits of manually lock mutexes and the benefits of a scoped lock
	// Watch out: lock is performed by the constructor. Do not manually re-lock it
	std::unique_lock<std::mutex> lockRunningGames{_accessRunningGames};
	std::unique_ptr<GameThread>& selfThread{_runningGames.at(idx)};
	// If the access did not throw, we can now release the mutex
	lockRunningGames.unlock();
	const auto& finderById = [](userId playerId)
	{
		return [playerId](const std::pair<const std::string, ClientInformations>& it)
		{
			return it.second.id == playerId;
		};
	};

	const auto& player1{std::find_if(_clients.begin(), _clients.end(), finderById(selfThread->_player1ID))};
	const auto& player2{std::find_if(_clients.begin(), _clients.end(), finderById(selfThread->_player2ID))};
	std::cout << "Game " << idx << " is starting: " + userToString(player1) + " vs. " + userToString(player2) + "\n";
	userId winnerId{selfThread->startGame(player1->second, player2->second)};
	// \TODO: change personnal scores
	const auto& winner{winnerId == selfThread->_player1ID ? player1 : player2};
	const auto& loser{winnerId == selfThread->_player1ID ? player2 : player1};
	std::cout << winner->first << " won and " << loser->first << " lost\n";
}

void Server::createGame(userId ID1, userId ID2)
{
	std::lock_guard<std::mutex> lockRunningGames{_accessRunningGames};
	_runningGames.emplace_back(new GameThread(_database, ID1, ID2, &Server::startGame, this, _runningGames.size()));
	// _accessRunningGames is unlocked when lockRunningGames is destructed
}

///////////////////////// Friends management

void Server::handleChatRequest(sf::Packet& packet, std::unique_ptr<sf::TcpSocket> client)
{
	sf::Packet responseToCaller;
	responseToCaller << TransferType::CHAT_PLAYER_IP;
	std::string calleeName;
	std::string callerName;
	sf::Uint16 callerPort;
	packet >> callerName >> calleeName >> callerPort;
	// first of all, verify that player exist
	// if it does, send his IP
	auto callee = _clients.find(calleeName);
	if(callee == _clients.end())
	{
		std::cout << "player does not exist!\n";
		responseToCaller << TransferType::FAILURE;
	}
	else
	{
		responseToCaller << TransferType::ACKNOWLEDGE << _clients[calleeName].socket->getRemoteAddress().toInteger();
		sf::Packet packetToCalle;
		sf::TcpSocket toCallee;
		if(toCallee.connect(_clients[calleeName].socket->getRemoteAddress(), _clients[calleeName].listeningPort) != sf::Socket::Done)
			std::cerr << "Unable to connect to callee (" << calleeName << ")\n";
		else
		{
			std::cout << "connected\n";
			// as the listening port has a delay, set communications as blokcing
			toCallee.setBlocking(true);
			packetToCalle << TransferType::CHAT_PLAYER_IP << client->getRemoteAddress().toInteger() << callerPort << calleeName << callerName;
			std::cout << "sending to callee\n";
			if(toCallee.send(packetToCalle) != sf::Socket::Done)
				std::cerr << "unable to send to calle\n";
			else
				std::cout << "sent to callee (" << calleeName << ")\n";
		}
	}
	client->send(responseToCaller);
}

void Server::handleFriendshipRequest(const _iterator& it, sf::Packet& transmission)
{
	sf::Packet response;
	std::string friendName;
	transmission >> friendName;
	try
	{
		const userId thisId{_database.getUserId(it->first)};
		const userId friendId{_database.getUserId(friendName)};

		// Add the request into the database
		_database.addFriendshipRequest(thisId, friendId);

		// Send an acknowledgement to the user
		response << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "handleFriendshipRequest error: " << e.what() << "\n";
		// Send an error to the user
		response << TransferType::NOT_EXISTING_FRIEND;
	}
	it->second.socket->send(response);
}

void Server::handleFriendshipRequestResponse(const _iterator& it, sf::Packet& transmission)
{
	bool accepted;
	std::string askerName;
	transmission >> askerName >> accepted;
	transmission.clear();
	try
	{
		const userId askerId{_database.getUserId(askerName)};
		const userId askedId{_database.getUserId(it->first)};
		if(not _database.isFriendshipRequestSent(askerId, askedId))
		{
			transmission << TransferType::NOT_EXISTING_FRIEND;
			throw std::runtime_error(userToString(it) + " responded to a friend request of an unexisting player.");
		}
		if(accepted)
			_database.addFriend(askerId, askedId);
		else
			_database.removeFriendshipRequest(askerId, askedId);

		// acknowledge to client
		transmission << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		// If the packet is empty, then _database threw
		if(transmission.getDataSize() == 0)
			transmission << TransferType::FAILURE;
		std::cout << "handleFriendshipRequestResponse error: " << e.what() << "\n";
	}
	it->second.socket->send(transmission);
}

void Server::sendFriendshipRequests(const _iterator& it)
{
	sf::Packet response;
	try
	{
		const userId id{_database.getUserId(it->first)};
		// The follwing two lines could be gathered, but by splitting them
		// we avoid that the packet is garbaged if ServerDatabase::getFriendshipRequests
		// throw an exception (although I don't think this is really risky,
		// it's better to be sure).
		FriendsList requests{_database.getFriendshipRequests(id)};
		response << TransferType::ACKNOWLEDGE << requests;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendFriendshipRequests error: " << e.what() << "\n";
		response << TransferType::FAILURE;
	}
	it->second.socket->send(response);
}

void Server::sendFriends(const _iterator& it)
{
	sf::Packet response;
	try
	{
		const userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		FriendsList friends{_database.getFriendsList(id)};
		response << TransferType::ACKNOWLEDGE << friends;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendFriends error: " << e.what() << "\n";
		response << TransferType::FAILURE;
	}
	it->second.socket->send(response);
}

void Server::handleRemoveFriend(const _iterator& it, sf::Packet& transmission)
{
	std::string removedFriend;
	transmission >> removedFriend;
	transmission.clear();
	try
	{
		const userId unfriendlyUserId{_database.getUserId(it->first)};
		const userId removedFriendId{_database.getUserId(removedFriend)};
		_database.removeFriend(unfriendlyUserId, removedFriendId);

		// acknowledge to client
		transmission << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		transmission << TransferType::NOT_EXISTING_FRIEND;
		std::cout << "handleRemoveFriend error: " << e.what() << "\n";
	}
	it->second.socket->send(transmission);
}

// Cards management

void Server::sendDecks(const _iterator& it)
{
	sf::Packet response;
	try
	{
		const userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		std::vector<Deck> decks{_database.getDecks(id)};
		response << TransferType::ACKNOWLEDGE << decks;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendDecks error: " << e.what() << "\n";
		response << TransferType::FAILURE;
	}
	it->second.socket->send(response);
}

void Server::handleDeckEditing(const _iterator& it, sf::Packet& transmission)
{
	Deck editedDeck;
	transmission >> editedDeck;
	transmission.clear();
	try
	{
		const userId id{_database.getUserId(it->first)};
		_database.editDeck(id, editedDeck);
		transmission << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "handleDeckEditing error: " << e.what() << "\n";
		transmission << TransferType::FAILURE;
	}
	it->second.socket->send(transmission);
}

void Server::handleDeckCreation(const _iterator& it, sf::Packet& transmission)
{
	Deck newDeck;
	transmission >> newDeck;
	transmission.clear();
	try
	{
		const userId id{_database.getUserId(it->first)};
		_database.createDeck(id, newDeck);
		transmission << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "handleDeckCreation error: " << e.what() << "\n";
		transmission << TransferType::FAILURE;
	}
	it->second.socket->send(transmission);
}

void Server::handleDeckDeletion(const _iterator& it, sf::Packet& transmission)
{
	std::string deletedDeckName;
	transmission >> deletedDeckName;
	transmission.clear();
	try
	{
		const userId id{_database.getUserId(it->first)};
		_database.deleteDeckByName(id, deletedDeckName);
		transmission << TransferType::ACKNOWLEDGE;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "handleDeckCreation error: " << e.what() << "\n";
		transmission << TransferType::FAILURE;
	}
	it->second.socket->send(transmission);
}

void Server::sendCardsCollection(const _iterator& it)
{
	sf::Packet response;
	try
	{
		const userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		CardsCollection cards{_database.getCardsCollection(id)};
		response << TransferType::ACKNOWLEDGE << cards;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendCardsCollection error: " << e.what() << "\n";
		response << TransferType::FAILURE;
	}
	it->second.socket->send(response);
}

// Others

void Server::sendLadder(const _iterator& it)
{
	sf::Packet response;
	try
	{
		Ladder ladder{_database.getLadder()};
		response << TransferType::ACKNOWLEDGE << ladder;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendLadder error: " << e.what() << "\n";
		response << TransferType::FAILURE;
	}
	it->second.socket->send(response);
}
