#ifndef _DECK_COMMON_HPP
#define _DECK_COMMON_HPP

#include <array>
#include <string>
#include "common/Card.hpp"

/// A deck is a set of cards. There is a special deck, the default deck,
/// wich is just a predefined deck that the user gets when it suscribes to
/// Wizard Poker. All decks have the same amount of cards, this amount is
/// defined by Deck::size.
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
}

#endif  // _DECK_COMMON_HPP
