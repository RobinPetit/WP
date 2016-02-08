#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <SFML/Network.hpp>
#include <string>

class Connection
{
public:
	Connection(const std::string& name);

	bool connect(const sf::IpAddress& address, int port);

private:
	sf::TcpSocket _socket;
	std::string _name;
};

#endif /* _CONNECTION_HPP_ */
