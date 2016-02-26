#ifndef _CARDS_COLLECTION_COMMON_HPP
#define _CARDS_COLLECTION_COMMON_HPP

#include <set>
#include "common/Identifiers.hpp"

class CardsCollection
{
	public:
		typedef std::multiset<cardId>::iterator Iterator;
		typedef std::multiset<cardId>::const_iterator ConstIterator;

		/// Default constructor.
		/// Initially, the card collection contains the first 20 cards.
		CardsCollection();

		CardsCollection(const CardsCollection& other) = default;

		explicit CardsCollection(const std::multiset<cardId>& cards);

		/// Add an occurence of \a card.
		/// \param card Card to add.
		void addCard(cardId card);

		/// Gives the number of occurences of \a card
		/// \param card Card to check.
		/// \return The number of occurences of \a card
		std::size_t count(cardId card);

		/// Checks if the set contains \a card.
		/// \param card Card to check.
		/// \return count(card) > 0
		bool contains(cardId card);

		std::size_t getSize() const;

		Iterator begin();

		Iterator end();

		ConstIterator cbegin() const;

		ConstIterator cend() const;

	private:
		std::multiset<cardId> _cards;  ///< The set of cards. A player can have multiple times a given card.
};

#endif  // _CARDS_COLLECTION_COMMON_HPP
