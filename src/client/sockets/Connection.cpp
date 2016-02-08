#include <client/sockets/Connection.hpp>
#include <common/constants.hpp>

Connection::Connection(const std::string& name)
{
	/* forces the name to not be larger than MAX_NAME_LENGTH */
	_name = (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
}

bool Connection::connect(const sf::IpAddress& address, int port)
{
	/* if connection does not work, don't go further */
	if(_socket.connect(address, port) != sf::Socket::Done)
		return false;
	/* do not forget the '\0' character */
	return _socket.send(_name.c_str(), _name.size()+1) == sf::Socket::Done;
}

