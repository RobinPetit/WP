#include <common/sockets/TransferType.hpp>

sf::Packet& operator<<(sf::Packet& packet, const TransferType& type)
{
    return packet << static_cast<const sf::Uint32>(type);
}

sf::Packet& operator>>(sf::Packet& packet, TransferType& type)
{
	sf::Uint32 v;
	sf::Packet& ret = packet >> v;
	type = static_cast<TransferType>(v);
	return ret;
}
