#include <algorithm>
#include <cassert>
#include "common/Deck.hpp"

Deck::Deck(const std::string& name):
	_name{name}
{
	for(std::size_t i{0}; i < size; ++i)
		_cards[i] = static_cast<CardId>(i+1);
}

Deck::Deck(const std::string& name, const std::array<CardId, size>& cards):
	_name{name},
	_cards{cards}
{
}

CardId Deck::getCard(std::size_t index) const
{
	return _cards[index];
}

std::size_t Deck::getIndex(CardId card) const
{
	auto it(std::find(_cards.begin(), _cards.end(), card));
	if(it == _cards.end())
		throw std::out_of_range("The card " + std::to_string(card) + " does not exist in this deck instance.");
	return static_cast<std::size_t>(std::distance(_cards.begin(), it));
}

void Deck::changeCard(std::size_t index, CardId card, bool checkWholeDeck)
{
	if(checkWholeDeck)
		assert(std::count(_cards.begin(), _cards.end(), card) < 2);
	else
		assert(std::count(_cards.begin(), _cards.begin() + index, card) < 2);
	_cards[index] = card;
}

const std::string& Deck::getName() const
{
	return _name;
}

void Deck::setName(const std::string& newName)
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

Deck::ConstIterator Deck::begin() const
{
	return _cards.cbegin();
}

Deck::ConstIterator Deck::end() const
{
	return _cards.cend();
}

Deck::ConstIterator Deck::cbegin() const
{
	return _cards.cbegin();
}

Deck::ConstIterator Deck::cend() const
{
	return _cards.cend();
}
