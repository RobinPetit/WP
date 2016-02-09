#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <SFML/Network.hpp>
#include <string>
#include <vector>

class Client
{
public:
	Client(const std::string& name);

	bool connectToServer(const sf::IpAddress& address, int port);
	bool startConversation(const std::string& playerName);

private:
	sf::TcpSocket _socket;
	std::string _name;
	std::vector<std::string> _currentConversations;
	bool _isConnected;
};

#endif // _CONNECTION_HPP_
