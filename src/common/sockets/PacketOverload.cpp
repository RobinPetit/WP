#include <algorithm>
#include "common/sockets/PacketOverload.hpp"

sf::Packet& operator <<(sf::Packet& packet, const Friend& userFriend)
{
	return packet << static_cast<sf::Int64>(userFriend.id) << userFriend.name;
}

sf::Packet& operator >>(sf::Packet& packet, Friend& userFriend)
{
	sf::Int64 tmp;
	packet >> tmp >> userFriend.name;
	userFriend.id = static_cast<int64_t>(tmp);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const LadderEntry& ladderEntry)
{
	return packet << ladderEntry.name
	              << static_cast<sf::Uint32>(ladderEntry.victories)
	              << static_cast<sf::Uint32>(ladderEntry.defeats);
}

sf::Packet& operator >>(sf::Packet& packet, LadderEntry& ladderEntry)
{
	sf::Uint32 victories, defeats;
	packet >> ladderEntry.name >> victories >> defeats;
	ladderEntry.victories = static_cast<unsigned>(victories);
	ladderEntry.defeats = static_cast<unsigned>(defeats);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Deck& deck)
{
	packet << deck.getName();
	for(std::size_t i{0}; i < Deck::size; ++i)
		packet << static_cast<sf::Int64>(deck.getCard(i));
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Deck& deck)
{
	std::string name;
	packet >> name;
	deck.setName(name);
	for(std::size_t i{0}; i < Deck::size; ++i)
	{
		sf::Int64 card;
		packet >> card;
		deck.changeCard(i, static_cast<cardId>(card));
	}
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const CardsCollection& cardCollection)
{
	packet << static_cast<sf::Uint32>(cardCollection.getSize());

	// This array is used to ensure that the 20 first cards are added into
	// the packet one less time than their actual number in the collection.
	// This is because the default-constructed card collection always have these
	// 20 cards, if we don't care they will be added one more time than needed.
	std::array<bool, Deck::size + 1> baseCardsAlreadyProcessed;
	std::fill(baseCardsAlreadyProcessed.begin(), baseCardsAlreadyProcessed.end(), false);

	for(auto it = cardCollection.cbegin(); it != cardCollection.cend(); ++it)
	{
		const cardId card{*it};
		// If this is a base card not already processed
		if(card > 0 and card < baseCardsAlreadyProcessed.size() and not baseCardsAlreadyProcessed[card])
			baseCardsAlreadyProcessed[card] = true;
		// else we can safely add it to the packet;
		else
			packet << static_cast<sf::Int64>(card);
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, CardsCollection& cardCollection)
{
	sf::Uint32 size;
	packet >> size;
	for(sf::Uint32 i{0}; i < size; ++i)
	{
		sf::Int64 card;
		packet >> card;
		cardCollection.addCard(static_cast<cardId>(card));
	}
	return packet;
}
