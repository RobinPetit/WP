#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>
// WizardPoker headers
#include "server/GameThread.hpp"
// std-C++ headers
#include <unordered_map>
#include <string>
#include <atomic>
#include <thread>

/// structure used inside of the server program to keep informations
/// on a single client
struct ClientInformations
{
	sf::TcpSocket *socket;
	sf::Uint16 listeningPort;  // used to send connection for the chat
	std::vector<std::string> friendshipRequests;  // players client asked to become friends with
	std::vector<std::string> acceptedRequests;  // players who accepted the friendship request
	std::vector<std::string> refusedRequests;  // players who refused the friendship request
	std::vector<std::string> externalRequests;  // players who asked to become friend with client
	std::vector<std::string> friends;
	std::size_t id;
};

class Server final
{
public:
	/// Constructor
	Server();

	/// Function used to start the server: it starts to listen and then to handle incoming packets
	/// \param listenerPort The port the server must be listening on
	int start(const sf::Uint16 listenerPort);

	/// Allows to free correctly the connected sockets
	void quit();

	/// Destructor
	~Server();

private:
	typedef std::unordered_map<std::string, ClientInformations>::iterator _iterator;

	// attributes
	std::unordered_map<std::string, ClientInformations> _clients;
	sf::TcpListener _listener;
	sf::SocketSelector _socketSelector;
	std::atomic_bool _done;
	std::atomic_bool _threadRunning;
	std::thread _quitThread;
	std::string _waitingPlayer;
	bool _isAPlayerWaiting;
	const std::string _quitPrompt;
	std::vector<GameThread *> _runningGames;
	std::size_t _last_id;

	// private methods
	/// Used to handle a newconnection request (when the listener gets a packet)
	void takeConnection();
	/// Used to handle data sent by a logged user
	void receiveData();

	/// Handle the input in stdin and quit the server if asked
	void waitQuit();

	// Friends management

	/// Used to exchange the correct informations with the clients when a chat request is made
	void handleChatRequest(sf::Packet& packet, sf::TcpSocket& client);
	/// Used to remove a player from the server connection
	void removeClient(const _iterator& it);
	/// Used to tell whether or not a user is connected
	void checkPresence(const _iterator& it, sf::Packet& transmission);
	/// Used to send the list of friends of a user
	void sendFriends(const _iterator& it);
	/// Used to update the database when an user remove an entry in its friend list
	void handleRemoveFriend(const _iterator& it, sf::Packet& transmission);
	/// Used to update the internal data when a frienship request is made
	void handleFriendshipRequest(const _iterator& it, sf::Packet& transmission);
	/// Used to receive the answer of a friendship request
	void handleFriendshipRequestResponse(const _iterator& it, sf::Packet& transmission);
	/// Used to send to a client the friendship request he received
	void sendFriendshipRequests(const _iterator& it);
	/// Used to send to a client the current state of the friendship requests state
	void sendFriendshipRequestsState(const _iterator& it);

	// Game management

	/// Used when a player want to play with another player
	void findOpponent(const _iterator& it);
	///
	void startGame(std::size_t);
	///
	void createGame(unsigned, unsigned);
};

#endif // _SERVER_HPP_
