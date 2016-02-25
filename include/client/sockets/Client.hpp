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

/// Client is a class representing the state of the client program (not the user!)
class Client final
{
public:
	/// Constructor
	Client();

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

	/////////// Friends management

	/// The function used to start the process of making a new discussion with the chat program
	/// \param playerName The name of the player the client wants to discuss with
	/// \throw NotConnectedException if connectToServer has not been called before
	/// \throw std::runtime_error if connection or chatting is impossible
	void startConversation(const std::string& playerName) const;

	/// The function used to to get a list of the user's friends
	/// \return A vector of names representing all of the friends
	/// \throw NotConnectedException if connectToServer has not been called before
	const std::vector<std::string>& getFriends();

	/// The function used to get a list of the user's friends who are connected
	/// \return A vector of names representing all of the connected friends
	/// \throw NotConnectedException if connectedToServer has not been called before
	std::vector<std::string> getConnectedFriends();

	/// Function used to ask the friends list to the server. The list containing the
	/// names is the attribute _friends
	/// \throw NotConnectedException if connectToServer has not been called before
	void updateFriends();

	/// Functions used to send a friendship request to another player
	/// \param name The name of the player asked to become a new friend
	/// \throw std::runtime_error if request is not correct
	void askNewFriend(const std::string& name);

	/// Function used to tell the server that the request is accepted
	/// \param name The name of the player who sent the request
	/// \param accept True to accept the request, false to refuse it
	void acceptFriendshipRequest(const std::string& name, bool accept=true);

	/// Used to ask the server if new friendship requet arrived
	/// \return True if the data was correctly received and false otherwise
	/// \param newIncomingRequests A vector containing the names of the players asking to become friend
	bool getIncomingFriendshipRequests(std::vector<std::string>& newIncomingRequests);

	/// Used to ask the server if requests were accepted
	/// \return True if the data was correctly received and false otherwise
	/// \param acceptedSentRequests A vector containing the names of the players who accepted to become friend
	/// \param refusedSentRequests A vector containing the names of the players who refused to become friends
	bool updateFriendshipRequests(
		std::vector<std::string>& acceptedSentRequests,
		std::vector<std::string>& refusedSentRequests);

	/// Used to know if a particular player is a friend or not
	/// \return True if the player is a friend of the client and false otherwise
	/// \param name The name of the player whose friendship is tested
	bool isFriend(const std::string& name) const;

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

	/// Used to wait sleeping until the atomic boolean readyToPlay is set to true
	void waitTillReadyToPlay();

	/// Destructor
	~Client();

private:
	///////// Cleitn/Server related attributes
	/// The socket that's connected to the server
	sf::TcpSocket _socket;
	/// The port the client is waiting for chat connections on
	sf::Uint16 _chatListenerPort;
	/// Stores whether the client is already connected to the server or not
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

	///////// Friend related attributes

	/// \TODO use this!
	/// A list of the discussion that are currently being hold
	std::vector<std::string> _currentConversations;
	/// List containing the names of the friends
	std::vector<std::string> _friends;
	/// \TODO use this!
	/// List containing the names of the friendship requests the client made
	std::vector<std::string> _friendsRequests;

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

	// private methods
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
