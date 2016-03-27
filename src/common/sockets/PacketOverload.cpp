#include <algorithm>
#include "common/sockets/PacketOverload.hpp"

sf::Packet& operator <<(sf::Packet& packet, const Friend& userFriend)
{
	return packet << userFriend.id << userFriend.name;
}

sf::Packet& operator >>(sf::Packet& packet, Friend& userFriend)
{
	return packet >> userFriend.id >> userFriend.name;
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
		packet << deck.getCard(i);
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Deck& deck)
{
	std::string name;
	packet >> name;
	deck.setName(name);
	for(std::size_t i{0}; i < Deck::size; ++i)
	{
		CardId card;
		packet >> card;
		deck.changeCard(i, card, false);
	}
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const CardsCollection& cardCollection)
{
	// If the card collection have less than 20 cards (strange, bu we never know)
	// then send 0 card
	if(cardCollection.getSize() < Deck::size)
		return packet << static_cast<sf::Uint32>(0);

	// We don't send the 20 base cards, so substract size by 20
	packet << static_cast<sf::Uint32>(cardCollection.getSize() - Deck::size);

	// This array is used to ensure that the 20 first cards are added into
	// the packet one less time than their actual number in the collection.
	// This is because the default-constructed card collection always have these
	// 20 cards, if we don't care they will be added one more time than needed.
	std::array<bool, Deck::size + 1> baseCardsAlreadyProcessed;
	std::fill(baseCardsAlreadyProcessed.begin(), baseCardsAlreadyProcessed.end(), false);

	for(auto it = cardCollection.cbegin(); it != cardCollection.cend(); ++it)
	{
		const CardId card{*it};
		// If this is a base card not already processed
		if(card > 0 and static_cast<std::size_t>(card) < baseCardsAlreadyProcessed.size()
		   and not baseCardsAlreadyProcessed[static_cast<std::array<bool, Deck::size+1>::size_type>(card)])
			baseCardsAlreadyProcessed[static_cast<std::array<bool, Deck::size+1>::size_type>(card)] = true;
		// else we can safely add it to the packet;
		else
			packet << card;
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, CardsCollection& cardCollection)
{
	sf::Uint32 size;
	packet >> size;
	for(sf::Uint32 i{0}; i < size; ++i)
	{
		CardId card;
		packet >> card;
		cardCollection.addCard(card);
	}
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const BoardCreatureData& data)
{
	return packet << data.id
	              << static_cast<sf::Int64>(data.health)
	              << static_cast<sf::Int64>(data.attack)
	              << static_cast<sf::Int64>(data.shield)
	              << data.shieldType;
}

sf::Packet& operator >>(sf::Packet& packet, BoardCreatureData& data)
{
	sf::Int64 health, attack, shield;
	packet >> data.id >> health >> attack >> shield >> data.shieldType;
	data.health = static_cast<int>(health);
	data.attack = static_cast<int>(attack);
	data.shield = static_cast<int>(shield);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const CardToSelect& card)
{
	packet << static_cast<sf::Int32>(card);
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, CardToSelect& card)
{
	sf::Int32 card32;
	packet >> card32;
	card = static_cast<CardToSelect>(card32);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const CardData& data)
{
	return packet << data.id;
}

sf::Packet& operator >>(sf::Packet& packet, CardData& data)
{
	return packet >> data.id;
}

sf::Packet& operator <<(sf::Packet& packet, const EndGame& endGameInfo)
{
	return packet << static_cast<std::underlying_type<EndGame::Cause>::type>(endGameInfo.cause)
	              << endGameInfo.applyToSelf;
}

sf::Packet& operator >>(sf::Packet& packet, EndGame& endGameInfo)
{
	std::underlying_type<EndGame::Cause>::type convertedCause;
	packet >> convertedCause >> endGameInfo.applyToSelf;
	endGameInfo.cause = static_cast<EndGame::Cause>(convertedCause);
	return packet;
}

//Achievement
sf::Packet& operator <<(sf::Packet& packet, const Achievement& achievement)
{
	return packet << static_cast<sf::Int64>(achievement.id)
				  << achievement.currentProgress;
}

sf::Packet& operator >>(sf::Packet& packet, Achievement& achievement)
{
	packet >> achievement.id >> achievement.currentProgress;
	return packet;
}
