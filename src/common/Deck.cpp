#include <algorithm>
#include <cassert>
#include "common/Deck.hpp"

Deck::Deck(const std::string& name):
	_name{name}
{
	for(std::size_t i{0}; i < size; ++i)
		_cards[i] = static_cast<ClientCard::ID>(i);
}

Deck::Deck(const std::string& name, const std::array<ClientCard::ID, size>& cards):
	_name{name},
	_cards{cards}
{
}

ClientCard::ID Deck::getCard(std::size_t index) const
{
	return _cards[index];
}

void Deck::changeCard(std::size_t index, ClientCard::ID card)
{
	assert(std::count(_cards.begin(), _cards.end(), card) < 2);
	_cards[index] = card;
}

const std::string& Deck::getName() const
{
	return _name;
}

void Deck::setName(const std::string newName)
{
	_name = newName;
}

Deck::Iterator Deck::begin()
{
	return _cards.begin();
}

Deck::Iterator Deck::end()
{
	return _cards.end();
}
