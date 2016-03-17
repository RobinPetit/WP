#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

// SFML headers
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
// std-C++ headers
#include <string>
#include <vector>
#include <thread>
#include <atomic>
// WizardPoker headers
#include "common/Terminal.hpp"
#include "common/Database.hpp"  // FriendsList typedef
#include "common/CardsCollection.hpp"
#include "common/Deck.hpp"
#include "client/ClientDatabase.hpp"

/// Client is a class representing the state of the client program (not the user!)
class Client final
{
public:
	/// Constructor
	Client();

	/// Destructor
	~Client();

	/// The function used to make first connection with the game server
	/// \return True if the connection succeeded and false otherwise
	/// \param name The name of the user
	/// \param password its password
	/// \param address The address where the server stands
	/// \param port The port the server occupies
	void connectToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port);

	/// The function used to register an user to the server.
	/// The connection is closed as soon the method exits, a call to
	/// connectToServer need to be done in order to play the game.
	/// The method is static to enforce the fact that no data is stored in the
	/// Client instance when the user register to the server.
	/// \return True if the registering succeeded and false otherwise
	/// \param name The name of the user
	/// \param password its password
	/// \param address The address where the server stands
	/// \param port The port the server occupies
	static void registerToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port);

	/////////// Friends management

	/// The function used to start the process of making a new discussion with the chat program
	/// \param playerName The name of the player the client wants to discuss with
	/// \throw NotConnectedException if connectToServer has not been called before
	/// \throw std::runtime_error if connection or chatting is impossible
	void startConversation(const std::string& playerName) const;

	/// The function used to to get a list of the user's friends
	/// \return A vector of names representing all of the friends
	/// \throw NotConnectedException if connectToServer has not been called before
	const FriendsList& getFriends();

	/// The function used to get a list of the user's friends who are connected
	/// \return A vector of names representing all of the connected friends
	/// \throw NotConnectedException if connectedToServer has not been called before
	FriendsList getConnectedFriends();

	/// Used to get a list of the friendship requests.
	/// \return A vector of names representing all of the users that sent a request
	/// \throw NotConnectedException if connectToServer has not been called before
	const FriendsList& getFriendshipRequests();

	/// Functions used to send a friendship request to another player
	/// \param name The name of the player asked to become a new friend
	/// \throw std::runtime_error if request is not correct
	void sendFriendshipRequest(const std::string& name);

	/// Function used to tell the server that the request is accepted
	/// \param name The name of the player who sent the request
	/// \param accept True to accept the request, false to refuse it
	/// \return True if the player was successfully accepted or refused, and false otherwise
	void acceptFriendshipRequest(const std::string& name, bool accept = true);

	/// Used to remove a friend from the friends list
	/// \param name The name of the player to remove from the friends list
	/// \throw std::runtime_error if request is not correct
	void removeFriend(const std::string& name);

	////////// Game management

	/// \TODO Complete this method (and its signature)
	bool startGame();

	/// Used to ask (from the menus for instance) the socket used in game
	/// \throw std::runtime_error if the method is called and no game has started
	sf::TcpSocket& getGameSocket();

	/// Used to ask the socked used to listen to server special data
	/// \throw std::runtime_error if the method is called and no game has started
	sf::TcpSocket& getGameListeningSocket();

	/// The function used to rest assured all conections are stopped and the client is
	/// not waiting for entering chat connections anymore
	void quit();

	// Cards management

	/// Used when the user wants its decks list
	std::vector<Deck> getDecks();

	/// Used when the user wants to change the content of a deck
	void handleDeckEditing(const Deck& editedDeck);

	/// Used when the user wants to create a deck
	void handleDeckCreation(const Deck& createdDeck);

	/// Used when the user wants to delete a deck
	void handleDeckDeletion(const std::string& deletedDeckName);

	/// Used when the user wants its cards collection
	CardsCollection getCardsCollection();

	/// Count number of card templates in database
	cardId getNumberOfCards();

	/// Give greatest card id in database
	cardId getMaxCardId();

	/// Used by AbstractGame
	const CommonCardData* getCardData(cardId id);

	// Others

	/// Used when the user wants the ladder
	Ladder getLadder();

	/// Used to wait sleeping until the atomic boolean readyToPlay is set to true
	void waitTillReadyToPlay();

	const Terminal& getTerminal();

private:
	///////// Client/Server related attributes
	/// The socket that's connected to the server
	sf::TcpSocket _socket;
	/// The port the client is waiting for chat connections on
	sf::Uint16 _chatListenerPort;
	/// Stores whether the client is already connected to the server or not.
	/// We talk here about a socket connexion, not the authentication.
	bool _isConnected;
	/// The client's name
	std::string _name;
	/// Used to wait for new chat connections
	std::thread _listenerThread;
	/// Stores the address of the server (needed to start the chat)
	sf::IpAddress _serverAddress;
	/// Stores the port of the server (needed to start the chat)
	sf::Uint16 _serverPort;
	/// Used to communicate with the listening thread to tell it to stop
	std::atomic_bool _threadLoop;

	///////// General management attributes

	/// Gives informations about the terminal installed on the computer
	Terminal _userTerminal;
	/// Hold informations about the cards
	ClientDatabase _database;

	///////// Friend related attributes

	/// \TODO use this!
	/// A list of the discussion that are currently being hold
	std::vector<std::string> _currentConversations;
	/// List containing the names of the friends
	FriendsList _friends;
	/// List containing the names of the users that sent a friendship request to the client
	FriendsList _friendshipRequests;

	//////// Game related attributes

	/// Tell whether the client is currently playing or not
	std::atomic_bool _inGame;
	/// Socket used only when a game is started to communicate with the specialized thread in the server
	sf::TcpSocket _inGameSocket;
	/// Socket used to receive special data from the server game thread
	sf::TcpSocket _inGameListeningSocket;
	/// Name of the opponent when in game
	std::string _inGameOpponentName;
	/// Tells whether everything has been set up correctly and the user is ready to start
	std::atomic_bool _readyToPlay;

	/////////// private methods

	/// chatListening is the function used by the client to make a new thread listening for entring connections
	/// (players that want to make a discussion)
	void inputListening();
	/// This function is used to start the chat program with the proper parameters
	void startChat(sf::Packet& transmission);
	/// This function is used to make the proper exchanges with the srever when a game is started
	void initInGameConnection(sf::Packet& transmission);

	/// Used to know if a particular player is a friend or not
	/// \return True if the player is a friend of the client and false otherwise
	/// \param name The name of the player whose friendship is tested
	bool isFriend(const std::string& name) const;

	/// Function used to ask the friends list to the server. The list containing the
	/// names is the attribute _friends
	/// \throw NotConnectedException if connectToServer has not been called before
	void updateFriends();

	/// Used to ask the server for the requests list. The list containing the
	/// requests is _friendshipRequests
	void updateFriendshipRequests();

	/// The function user to connect to the server, used by both connectToServer
	/// and registerToServer. The connection is just a socket connection,
	/// the user is not authentificated with this method.
	/// \return True if the connection succeeded and false otherwise
	/// \param name The name of the user
	/// \param address The address where the server stands
	/// \param port The port the server occupies
	void initServer(const std::string& name, const sf::IpAddress& address, sf::Uint16 port);

	/// The function used to establish a connection in the user point of view,
	/// authentificating the user and allowing it to effectively play the game.
	/// \param password The user's password
	void sendConnectionToken(const std::string& password);

	/// The function used to register the user to the server.
	/// \param name The name of the user
	/// \param password The user's password
	static void sendRegisteringToken(const std::string& name, const std::string& password, sf::TcpSocket& socket);

	/// The functions used to create the listening thread
	void initListener();

	/// Forces the name to not be larger than MAX_NAME_LENGTH
	static std::string shrinkName(const std::string& name);
};

#endif // _CONNECTION_HPP_
