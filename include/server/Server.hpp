#ifndef _SERVER_HPP_
#define _SERVER_HPP_

// SFML headers
#include <SFML/Network.hpp>
// std-C++ headers
#include <unordered_map>
#include <string>

/// structure used isnide of the server program to keep informations
/// ona single client
struct ClientInformations
{
	sf::TcpSocket *socket;
	sf::Uint16 listeningPort;  // used to send connection for the chat
};

class Server
{
public:
	/// Constructor
	Server();

	/// Function used to start the server: it starts to listen and then to handle incoming packets
	/// \param listenerPort The port the server must be listening on
	int start(const unsigned short listenerPort);

private:
	// attributes
	std::unordered_map<std::string, ClientInformations> _clients;
	sf::TcpListener _listener;
	sf::SocketSelector _socketSelector;
	bool _done;

	// private methods
	/// Used to handle a newconnection request (when the listener gets a packet)
	void takeConnection();
	/// Used to handle data sent by a logged user
	void receiveData();
	/// Used to exchange the correct informations with the clients when a chat request is made
	void handleChatRequest(sf::Packet& packet, sf::TcpSocket& client);
};

#endif // _SERVER_HPP_
