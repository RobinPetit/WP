#ifndef _PACKET_OVERLOAD_HPP_
#define _PACKET_OVERLOAD_HPP_

// std-C++ headers
#include <vector>
// SFML headers
#include <SFML/Network/Packet.hpp>

template <typename T>
sf::Packet& operator <<(sf::Packet& packet, const std::vector<T>& vec);

template <typename T>
sf::Packet& operator >>(sf::Packet& packet, std::vector<T>& vec);


// implementation

template <typename T>
sf::Packet& operator <<(sf::Packet& packet, const std::vector<T>& vec)
{
	sf::Uint32 tmp = static_cast<sf::Uint32>(vec.size());
	packet << tmp;
	for(const auto& element: vec)
		packet << element;
	return packet;
}

template <typename T>
sf::Packet& operator >>(sf::Packet& packet, std::vector<T>& vec)
{
	sf::Uint32 tmp;
	typename std::vector<T>::size_type length;
	packet >> tmp;
	length = tmp;
	for(int i = 0; i < length; ++i)
	{
		T value;
		packet >> value;
		vec.push_back(value);
	}
	return packet;
}

#endif  // _PACKET_OVERLOAD_HPP_