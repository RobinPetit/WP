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
#include "common/Database.hpp"// FriendsList typedef
#include "common/CardsCollection.hpp"
#include "common/Deck.hpp"

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
	bool connectToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port);

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
	static bool registerToServer(const std::string& name, const std::string& password, const sf::IpAddress& address, sf::Uint16 port);

	// Friends management

	/// The function used to start the process of making a new discussion with the chat program
	/// \return True if the chat program has been started and false otherwise
	/// \param playerName The name of the player the client wants to discuss with
	/// \throw NotConnectedException if connectToServer has not been called before
	bool startConversation(const std::string& playerName) const;

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
	/// \return True if the player was successfully asked to become friend and false if he was already friend
	/// \param name The name of the player asked to become a new friend
	bool sendFriendshipRequest(const std::string& name);

	/// Function used to tell the server that the request is accepted
	/// \param name The name of the player who sent the request
	/// \param accept True to accept the request, false to refuse it
	/// \return True if the player was successfully accepted or refused, and false otherwise
	bool acceptFriendshipRequest(const std::string& name, bool accept=true);

	/// Used to remove a friend from the friends list
	/// \return True if the player was successfully removed and false if he is not a friend
	/// \param name The name of the player to remove from the friends list
	bool removeFriend(const std::string& name);

	// Game management
	/// \TODO Complete this method (and its signature)
	void startGame();

	/// The function used to rest assured all conections are stopped and the client is
	/// not waiting for entering chat connections anymore
	void quit();

	// Cards management

	/// Used when the user wants its decks list
	std::vector<Deck> getDecks();

	/// Used when the user wants to change the content of a deck
	bool handleDeckEditing(const Deck& editedDeck);

	/// Used when the user wants to create a deck
	bool handleDeckCreation(const Deck& createdDeck);

	/// Used when the user wants to delete a deck
	bool handleDeckDeletion(const std::string& deletedDeckName);

	/// Used when the user wants its cards collection
	CardsCollection getCardsCollection();

private:
	/// The socket that's connected to the server
	sf::TcpSocket _socket;
	/// The port the client is waiting for chat connections on
	sf::Uint16 _chatListenerPort;
	/// The client's name
	std::string _name;
	/// \TODO use this!
	/// A list of the discussion that are currently being hold
	std::vector<std::string> _currentConversations;
	/// Stores whether the client is already connected to the server or not
	bool _isConnected;
	/// Used to wait for new chat connections
	std::thread _listenerThread;
	/// Used to communicate with the listening thread to tell it to stop
	std::atomic_bool _threadLoop;
	/// Stores the address of the server (needed to start the chat)
	sf::IpAddress _serverAddress;
	/// Stores the port of the server (needed to start the chat)
	sf::Uint16 _serverPort;
	/// Gives informations about the terminal installed on the computer
	Terminal _userTerminal;
	/// List containing the names of the friends
	FriendsList _friends;
	/// List containing the names of the users that sent a friendship request to the client
	FriendsList _friendshipRequests;

	// private methods
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
	bool initServer(const std::string& name, const sf::IpAddress& address, sf::Uint16 port);

	/// The function used to establish a connection in the user point of view,
	/// authentificating the user and allowing it to effectively play the game.
	/// \param password The user's password
	bool sendConnectionToken(const std::string& password);

	/// The function used to register the user to the server.
	/// \param name The name of the user
	/// \param password The user's password
	static bool sendRegisteringToken(const std::string& name, const std::string& password, sf::TcpSocket& socket);

	/// The functions used to create the listening thread
	void initListener();

	/// Forces the name to not be larger than MAX_NAME_LENGTH
	static std::string shrinkName(const std::string& name);
};

#endif // _CONNECTION_HPP_
