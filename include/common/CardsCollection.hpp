#ifndef _CARDS_COLLECTION_COMMON_HPP
#define _CARDS_COLLECTION_COMMON_HPP

#include <set>
#include "client/Card.hpp"

class CardsCollection
{
	public:
		typedef std::multiset<ClientCard::ID>::iterator Iterator;
		typedef std::multiset<ClientCard::ID>::const_iterator ConstIterator;

		/// Default constructor.
		/// Initially, the card collection contains the first 20 cards.
		CardsCollection();

		CardsCollection(const CardsCollection& other) = default;

		explicit CardsCollection(const std::multiset<ClientCard::ID>& cards);

		/// Add an occurence of \a card.
		/// \param card Card to add.
		void addCard(ClientCard::ID card);

		/// Gives the number of occurences of \a card
		/// \param card Card to check.
		/// \return The number of occurences of \a card
		std::size_t count(ClientCard::ID card);

		/// Checks if the set contains \a card.
		/// \param card Card to check.
		/// \return count(card) > 0
		bool contains(ClientCard::ID card);

		std::size_t getSize() const;

		Iterator begin();

		Iterator end();

		ConstIterator cbegin() const;

		ConstIterator cend() const;

	private:
		std::multiset<ClientCard::ID> _cards;  ///< The set of cards. A player can have multiple times a given card.
};

#endif  // _CARDS_COLLECTION_COMMON_HPP
