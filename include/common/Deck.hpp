#ifndef _DECK_COMMON_HPP
#define _DECK_COMMON_HPP

#include <array>
#include <string>
#include "common/Card.hpp"

/// A deck is a set of cards.
/// All decks have the same amount of card, defined by Deck::size.
/// There is a special deck, the default deck
/// The default deck contains exactly Deck::size unique cards that match
/// the Deck::size cards all players start with.
// \TODO Card.hpp should implements a type CardId
// (maybe such as `typedef std::size_t CardId`)
class Deck
{
	public:
		static constexpr std::size_t size{20};

		/// Copy constructor.
		Deck(const Deck& other) = default;

		/// Destructor.
		virtual ~Deck();

		/// Constructor.
		/// Creates a default deck. The default deck is the set of the \a size
		/// first cards (with the cards ordered by their ID).
		explicit Deck(const std::string& name);

		/// Constructor.
		/// Creates a deck from an array of cards.
		Deck(const std::string& name, const std::array<CardId, size>& cards);

		/// Replaces the card at \a index in the deck by \a card.
		void changeCard(std::size_t index, const CardId& card);

	private:
		std::string _name;              ///< The name of the deck.
		std::array<CardId, size> _cards;///< All the cards of the deck.
};

#endif  // _DECK_COMMON_HPP
