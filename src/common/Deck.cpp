#include <algorithm>
#include <cassert>
#include "common/Deck.hpp"

Deck::Deck(const std::string& name):
	_name{name}
{
	for(std::size_t i{0}; i < size; ++i)
		_cards[i] = static_cast<CardId>(i);
}

Deck::Deck(const std::string& name, const std::array<CardId, size>& cards):
	_name{name},
	_cards{cards}
{
}

void Deck::changeCard(std::size_t index, CardId card)
{
	assert(std::count(_cards.begin(), _cards.end(), card) < 2);
	_cards[index] = card;
}
