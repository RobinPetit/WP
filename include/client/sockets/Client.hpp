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

/// Client is a class representing the state of the client program (not the user!)
class Client
{
public:
	/// Constructor
	/// \param name The name of the program (a priori the same as the user's)
	Client(const std::string& name);

	/// The functions used to make first connection with the game server
	/// \return True if the connection succeeded and false otherwise
	/// \param address The address where the server stands
	/// \param port The port the server occupies
	bool connectToServer(const sf::IpAddress& address, sf::Uint16 port);

	/// The fonctions used to start the process of making a new discussion with the chat program
	/// \return True if the chat program has been started and false otherwise
	/// \param playerName The name of the player the client wants to discuss with
	bool startConversation(const std::string& playerName);

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

	// private methods
	/// The functions used to create the listening thread
	void initListener();
};

#endif // _CONNECTION_HPP_
