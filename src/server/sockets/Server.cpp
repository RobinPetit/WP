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
	_database(),
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
	std::unique_ptr<sf::TcpSocket> newClient{new sf::TcpSocket()};
	// if listener can't accept correctly, free the allocated socket
	if(_listener.accept(*newClient) != sf::Socket::Done)
	{
		std::cout << "Error when trying to accept a new client.\n";
		return;
	}
	// receive username
	sf::Packet packet;
	newClient->receive(packet);
	TransferType type;
	packet >> type;
	if(type == TransferType::GAME_CONNECTION)
		connectUser(packet, std::move(newClient));
	else if(type == TransferType::GAME_REGISTERING)
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
	bool failedToConnect{false};
	// N + 1/2 loop (the 1/2 is the error handling)
	do
	{
		try
		{
			connectionPacket >> playerName >> password >> clientPort;
			connectionPacket.clear();

			// Check if the user is not already connected
			if(_clients.find(playerName) != _clients.end())
			{
				connectionPacket << TransferType::GAME_ALREADY_CONNECTED;
				throw std::runtime_error(playerName + " tried to connect to the server but is already connected.");
			}

			if(false /* UNCOMMENT _database.areIdentifiersValid(playerName, password) */)
			{
				connectionPacket << TransferType::GAME_WRONG_IDENTIFIERS;
				throw std::runtime_error(playerName + " gives wrong identifiers when trying to connect.");
			}
			failedToConnect = false;
		}
		catch(const std::runtime_error& e)
		{
			// Send a response indicating the error
			client->send(connectionPacket);

			std::cout << "connectUser error: " << e.what() << "\n";
			failedToConnect = true;

			// Receive the new try of the client
			client->receive(connectionPacket);
			TransferType type;
			connectionPacket >> type;
			if(type != TransferType::GAME_CONNECTION)
			{
				std::cout << "connectUser error: wrong packet transmitted after failed connection (expecting another connection packet).\n";
				// The socket will automatically be closed, thanks to smart pointers
				return;
			}
		}
	} while(failedToConnect);

	std::cout << "New player connected: " << playerName << std::endl;
	sendAcknowledgement(*client);
	// add this client to the selector so that its receivals are handled properly
	// (be sure that this line is before the next, otherwise we get a segfault)
	_socketSelector.add(*client);
	// and add the new socket to the clients
	_clients[playerName] = {std::move(client), clientPort, ++_last_id};
}

void Server::registerUser(sf::Packet& registeringPacket, std::unique_ptr<sf::TcpSocket> client)
{
	std::string playerName, password;
	bool failedToRegister;
	// N + 1/2 loop (the 1/2 is the error handling)
	do
	{
		try
		{
			registeringPacket >> playerName >> password;
			registeringPacket.clear();

			if(false /* UNCOMMENT _database.isRegistered(playerName) */)
			{
				registeringPacket << TransferType::GAME_USERNAME_NOT_AVAILABLE;
				throw std::runtime_error(playerName + " tried to register to the server but the name is not available.");
			}
			// UNCOMMENT _database.registerUser(playerName, password);
			failedToRegister = false;
		}
		catch(const std::runtime_error& e)
		{
			// If Database::registerUser threw an exception, the packet is empty
			if(registeringPacket.getDataSize() == 0)
				registeringPacket << TransferType::GAME_FAILED_TO_REGISTER;

			std::cout << "registerUser error: " << e.what() << "\n";
			failedToRegister = true;

			// Send a response indicating the error
			client->send(registeringPacket);

			// Receive the new try of the client
			client->receive(registeringPacket);
			TransferType type;
			registeringPacket >> type;
			if(type != TransferType::GAME_REGISTERING)
			{
				std::cout << "registerUser error: wrong packet transmitted after failed registering (expecting another registering packet).\n";
				return;
			}
		}
	} while(failedToRegister);

	sendAcknowledgement(*client);
	std::cout << "New player registered: " << playerName << std::endl;
}
void Server::sendAcknowledgement(sf::TcpSocket& client)
{
	sf::Packet packet;
	packet << TransferType::ACKNOWLEDGE;
	client.send(packet);
}

void Server::receiveData()
{
	std::cout << "Data received.\n";
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
		case TransferType::PLAYER_GETTING_FRIEND_REQUESTS:
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
		case TransferType::PLAYER_ASKS_DECKS_LIST:
			sendDecks(it);
			break;
		case TransferType::PLAYER_EDIT_DECK:
			handleDeckEditing(it, packet);
			break;
		case TransferType::PLAYER_CREATE_DECK:
			handleDeckCreation(it, packet);
			break;
		case TransferType::PLAYER_DELETE_DECK:
			handleDeckDeletion(it, packet);
			break;
		case TransferType::PLAYER_ASKS_CARDS_COLLECTION:
			sendCardsCollection(it);
			break;
		// Others
		case TransferType::PLAYER_ASKS_LADDER:
			sendLadder(it);
			break;
		default:
			std::cerr << "Error: unknown code " << static_cast<sf::Uint32>(type) << std::endl;
			break;
		}
	}
	else if(receivalStatus == sf::Socket::Disconnected)
	{
		std::cerr << "Connection with player " << it->first << " is lost: forced disconnection from server." << std::endl;
		removeClient(it);
	}
	else
		std::cerr << "data not well received." << std::endl;
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
		delete gameThread;
	}
	_runningGames.clear();
	// in case the method is called even though server has not been manually ended
	_done.store(true);
	_quitThread.join();
	_threadRunning.store(false);
	_socketSelector.clear();
	_clients.clear();
	// leave listening port
	_listener.close();
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

//////////////// Game management

void Server::findOpponent(const _iterator& it)
{
	_lobbyMutex.lock();
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
	_lobbyMutex.unlock();
}

void Server::clearLobby(const _iterator& it)
{
	_lobbyMutex.lock();
	if(not _isAPlayerWaiting or _waitingPlayer != it->first)
	{
		std::cerr << "Trying to remove another player from lobby; ignored\n";
		return;
	}
	else
		_isAPlayerWaiting = false;
	_lobbyMutex.unlock();
}

void Server::startGame(std::size_t idx)
{
	std::cout << "StartGame(" << idx << ")\n";
	_accessRunningGames.lock();
	GameThread& selfThread{*_runningGames[idx]};
	_accessRunningGames.unlock();
	const auto& finderById = [](unsigned playerId)
	{
		return [playerId](const std::pair<const std::string, ClientInformations>& it)
		{
			return it.second.id == playerId;
		};
	};
	const auto& player1 = std::find_if(_clients.begin(), _clients.end(), finderById(selfThread._player1ID));
	const auto& player2 = std::find_if(_clients.begin(), _clients.end(), finderById(selfThread._player2ID));
	std::cout << "Game " << idx << " is starting: " << player1->first << " vs. " << player2->first << "\n";
	selfThread.startGame(player1->second, player2->second);
}

void Server::createGame(unsigned ID1, unsigned ID2)
{
	_accessRunningGames.lock();
	_runningGames.emplace_back(new GameThread(ID1, ID2, &Server::startGame, this, _runningGames.size()));
	_accessRunningGames.unlock();
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
		const Database::userId thisId{_database.getUserId(it->first)};
		const Database::userId friendId{_database.getUserId(friendName)};

		// Add the request into the database
		// UNCOMMENT _database.addFriendRequest(thisId, friendId);

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
		const Database::userId askerId{_database.getUserId(askerName)};
		const Database::userId askedId{_database.getUserId(it->first)};
		if(not true /* UNCOMMENT _database.hasSentRequest(askerId, askedId) */)
		{
			transmission << TransferType::NOT_EXISTING_FRIEND;
			throw std::runtime_error(it->first + " responded to a friend request of an unexisting player.");
		}
		if(accepted)
			_database.addFriend(askerId, askedId);
		else
		{
			/* _database.removeFriendshipRequest(askerId, askedId) */;
		}

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
		const Database::userId id{_database.getUserId(it->first)};
		// The follwing two lines could be gathered, but by splitting them
		// we avoid that the packet is garbaged if ServerDatabase::getFriendshipRequests
		// throw an exception (although I don't think this is really risky,
		// it's better to be sure).
		FriendsList requests{_database.getFriendshipRequests(id)};
		response << requests;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendFriendshipRequests error: " << e.what() << "\n";
		response << FriendsList();
	}
	it->second.socket->send(response);
}

void Server::sendFriends(const _iterator& it)
{
	sf::Packet response;
	try
	{
		const Database::userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		FriendsList friends{_database.getFriendsList(id)};
		response << friends;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendFriends error: " << e.what() << "\n";
		response << FriendsList();
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
		const Database::userId unfriendlyUserId{_database.getUserId(it->first)};
		const Database::userId removedFriendId{_database.getUserId(removedFriend)};
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
		const Database::userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		std::vector<Deck> decks{_database.getDecks(id)};
		response << decks;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendDecks error: " << e.what() << "\n";
		response << std::vector<Deck>();
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
		const Database::userId id{_database.getUserId(it->first)};
		// UNCOMMENT _database.editDeck(id, editedDeck);
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
		const Database::userId id{_database.getUserId(it->first)};
		// UNCOMMENT _database.createDeck(id, newDeck);
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
		const Database::userId id{_database.getUserId(it->first)};
		// UNCOMMENT _database.deleteDeck(id, deletedDeckName);
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
		const Database::userId id{_database.getUserId(it->first)};
		// Same as sendFriendshipRequests for the two folling lines
		CardsCollection cards{_database.getCardsCollection(id)};
		response << cards;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendCardsCollection error: " << e.what() << "\n";
		response << CardsCollection();
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
		response << ladder;
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "sendLadder error: " << e.what() << "\n";
		response << Ladder();
	}
	it->second.socket->send(response);
}
