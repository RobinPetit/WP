#include "common/sockets/PacketOverload.hpp"

sf::Packet& operator <<(sf::Packet& packet, const Friend& userFriend)
{
	packet << static_cast<sf::Int64>(userFriend.id) << userFriend.name;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Friend& userFriend)
{
	sf::Int64 tmp;
	packet >> tmp >> userFriend.name;
	userFriend.id = static_cast<int64_t>(tmp);
	return packet;
}
