#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

// SFML headers
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
// std-C++ headers
#include <string>
#include <vector>
#include <thread>
#include <atomic>
// WizardPoker headers
#include <common/Terminal.hpp>

/// Client is a class representing the state of the client program (not the user!)
class Client final
{
public:
	/// Constructor
	/// \param name The name of the program (a priori the same as the user's)
	Client(const std::string& name);

	/// The function used to make first connection with the game server
	/// \return True if the connection succeeded and false otherwise
	/// \param address The address where the server stands
	/// \param port The port the server occupies
	bool connectToServer(const sf::IpAddress& address, sf::Uint16 port);

	/// The function used to start the process of making a new discussion with the chat program
	/// \return True if the chat program has been started and false otherwise
	/// \param playerName The name of the player the client wants to discuss with
	/// \throw NotConnectedException if connectToServer has not been called before
	bool startConversation(const std::string& playerName);

	/// The function used to to get a list of the user's friends
	/// \return A vector of names representing all of the friends
	/// \param onlyConnected Boolean precising if non-connected players must be considered or not
	/// \throw NotConnectedException if onlyConnected is set to true
	/// and connectToServer has not been called before
	std::vector<std::string> getFriends(bool onlyConnected=true);

	/// Function used to ask the friends list to the server. The list containing the
	/// names is the attribute _friends
	/// \throw NotConnectedException if connectToServer has not been called before
	void updateFriends();

	/// The function used to rest assured all conections are stopped and the client is
	/// not waiting for entering chat connections anymore
	void quit();

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
	std::vector<std::string> _friends;
	/// \TODO use this!
	/// List  containing the names of the friendship requests
	std::vector<std::string> _friendsRequest;

	// private methods
	/// The functions used to create the listening thread
	void initListener();
};

#endif // _CONNECTION_HPP_
