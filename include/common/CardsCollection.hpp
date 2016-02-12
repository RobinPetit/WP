#ifndef _CARDS_COLLECTION_COMMON_HPP
#define _CARDS_COLLECTION_COMMON_HPP

#include <set>
#include "common/Card.hpp"

class CardsCollection
{
	public:
		/// Default constructor.
		/// Initially, the card collection contains the first 20 cards.
		CardsCollection();

		CardsCollection(const CardsCollection& other) = default;

		explicit CardsCollection(const std::multiset<CardId>& cards);

		/// Add an occurence of \a card.
		/// \param card Card to add.
		void addCard(CardId card);

		/// Gives the number of occurences of \a card
		/// \param card Card to check.
		/// \return The number of occurences of \a card
		std::size_t count(CardId card);

		/// Checks if the set contains \a card.
		/// \param card Card to check.
		/// \return count(card) > 0
		bool contains(CardId card);

	private:
		std::multiset<CardId> _cards;///< The set of cards. A player can have multiple times a given card.
};

#endif  // _CARDS_COLLECTION_COMMON_HPP
