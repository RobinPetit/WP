#include <algorithm>
#include <cassert>
#include "common/Deck.hpp"

Deck::Deck()
{
	for(std::size_t i{0}; i < size; ++i)
		_cards[i] = static_cast<Card::ID>(i);
}

Deck::Deck(const std::array<Card::ID, size>& cards):
	_cards{cards}
{
}

void Deck::changeCard(std::size_t index, Card::ID card)
{
	assert(std::count(_cards.begin(), _cards.end(), card) < 2);
	_cards[index] = card;
}
