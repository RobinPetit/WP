#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

CardsCollection::CardsCollection()
{
	for(std::size_t i{1}; i <= Deck::size; ++i)
		_cards.insert(static_cast<ClientCard::ID>(i));
}

CardsCollection::CardsCollection(const std::multiset<ClientCard::ID>& cards):
	_cards{cards}
{
}

void CardsCollection::addCard(ClientCard::ID card)
{
	_cards.insert(card);
}

std::size_t CardsCollection::count(ClientCard::ID card)
{
	return _cards.count(card);
}

bool CardsCollection::contains(ClientCard::ID card)
{
	return count(card) > 0;
}

std::size_t CardsCollection::getSize() const
{
	return _cards.size();
}

CardsCollection::Iterator CardsCollection::begin()
{
	return _cards.begin();
}

CardsCollection::Iterator CardsCollection::end()
{
	return _cards.end();
}

CardsCollection::ConstIterator CardsCollection::cbegin() const
{
	return _cards.cbegin();
}

CardsCollection::ConstIterator CardsCollection::cend() const
{
	return _cards.cend();
}
