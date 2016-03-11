#ifndef _PACKET_OVERLOAD_HPP_
#define _PACKET_OVERLOAD_HPP_

// std-C++ headers
#include <vector>
// SFML headers
#include <SFML/Network/Packet.hpp>
// WizardPoker header
#include "common/Database.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"
#include "common/GameData.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/EndGame.hpp"

template <typename T>
sf::Packet& operator <<(sf::Packet& packet, const std::vector<T>& vec);

template <typename T>
sf::Packet& operator >>(sf::Packet& packet, std::vector<T>& vec);

template <typename T, std::size_t N>
sf::Packet& operator <<(sf::Packet& packet, const std::array<T, N>& array);

template <typename T, std::size_t N>
sf::Packet& operator >>(sf::Packet& packet, std::array<T, N>& array);

sf::Packet& operator <<(sf::Packet& packet, const Friend& userFriend);

sf::Packet& operator >>(sf::Packet& packet, Friend& userFriend);

sf::Packet& operator <<(sf::Packet& packet, const LadderEntry& ladderEntry);

sf::Packet& operator >>(sf::Packet& packet, LadderEntry& ladderEntry);

sf::Packet& operator <<(sf::Packet& packet, const Deck& deck);

sf::Packet& operator >>(sf::Packet& packet, Deck& deck);

sf::Packet& operator <<(sf::Packet& packet, const CardsCollection& deck);

sf::Packet& operator >>(sf::Packet& packet, CardsCollection& deck);

/// Allow a packet to transmit a BoardCreatureData instance.
sf::Packet& operator <<(sf::Packet& packet, const BoardCreatureData& data);

sf::Packet& operator >>(sf::Packet& packet, BoardCreatureData& data);

sf::Packet& operator <<(sf::Packet& packet, const CardToSelect& card);

sf::Packet& operator >>(sf::Packet& packet, CardToSelect& card);

/// Allow a packet to transmit a CardData instance
sf::Packet& operator <<(sf::Packet& packet, const CardData& data);

sf::Packet& operator >>(sf::Packet& packet, CardData& data);

sf::Packet& operator <<(sf::Packet& packet, const EndGame& endGameInfo);

sf::Packet& operator >>(sf::Packet& packet, EndGame& endGameInfo);

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
	for(typename std::vector<T>::size_type i = 0; i < length; ++i)
	{
		T value;
		packet >> value;
		vec.push_back(value);
	}
	return packet;
}

template <typename T, std::size_t N>
sf::Packet& operator <<(sf::Packet& packet, const std::array<T, N>& array)
{
	// Damn, this code is so hard!
	for(const auto& value : array)
		packet << value;
	return packet;
}

template <typename T, std::size_t N>
sf::Packet& operator >>(sf::Packet& packet, std::array<T, N>& array)
{
	for(std::size_t i{0}; i < N; ++i)
		packet >> array[i];
	return packet;
}

#endif  // _PACKET_OVERLOAD_HPP_
