#ifndef _CARDS_COLLECTION_COMMON_HPP
#define _CARDS_COLLECTION_COMMON_HPP

#include <set>
#include "common/Card.hpp"

class CardsCollection
{
	public:
		typedef std::multiset<Card::ID>::iterator Iterator;

		/// Default constructor.
		/// Initially, the card collection contains the first 20 cards.
		CardsCollection();

		CardsCollection(const CardsCollection& other) = default;

		explicit CardsCollection(const std::multiset<Card::ID>& cards);

		/// Add an occurence of \a card.
		/// \param card Card to add.
		void addCard(Card::ID card);

		/// Gives the number of occurences of \a card
		/// \param card Card to check.
		/// \return The number of occurences of \a card
		std::size_t count(Card::ID card);

		/// Checks if the set contains \a card.
		/// \param card Card to check.
		/// \return count(card) > 0
		bool contains(Card::ID card);

		Iterator begin();

		Iterator end();

	private:
		std::multiset<Card::ID> _cards;  ///< The set of cards. A player can have multiple times a given card.
};

#endif  // _CARDS_COLLECTION_COMMON_HPP
