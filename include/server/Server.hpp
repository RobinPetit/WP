#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <SFML/Network.hpp>
#include <unordered_map>
#include <string>

class Server
{
public:
	Server();

	int start(const unsigned short listenerPort);

private:
	// attributes
	std::unordered_map<std::string, sf::TcpSocket *> _clients;
	sf::TcpListener _listener;
	sf::SocketSelector _socketSelector;
	bool _done;

	// private methods
	void takeConnection();
	void receiveData();
	void handleChatRequest(sf::Packet& packet, sf::TcpSocket& client);
};

#endif // _SERVER_HPP_
