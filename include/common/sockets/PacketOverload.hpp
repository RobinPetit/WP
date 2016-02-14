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
	packet << vec.size();
	for(const auto& element: vec)
		packet << element;
	return packet;
}

template <typename T>
sf::Packet& operator >>(sf::Packet& packet, std::vector<T>& vec)
{
	typename std::vector<T>::size_type length;
	packet >> length;
	for(int i = 0; i < length; ++i)
	{
		T value;
		packet >> value;
		vec.push_back(value);
	}
	return packet;
}


#endif  // _PACKET_OVERLOAD_HPP_
