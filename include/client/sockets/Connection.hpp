#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <SFML/Network.hpp>

class Connection
{
public:
	Connection(const sf::IpAddress&, int);

private:
	sf::IpAddress _ip;
	sf::TcpSocket _socket;
};

#endif /* _CONNECTION_HPP_ */
