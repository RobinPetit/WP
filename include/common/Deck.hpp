#ifndef _DECK_COMMON_HPP
#define _DECK_COMMON_HPP

#include <array>
#include <string>
#include "client/Card.hpp"

/// A deck is a set of cards.
/// All decks have the same amount of card, defined by Deck::size.
/// There is a special deck, the default deck
/// The default deck contains exactly Deck::size unique cards that match
/// the Deck::size cards all players start with.
// \TODO Card.hpp should implements a type ClientCard::ID
// (maybe such as `typedef std::size_t ClientCard::ID`)
class Deck
{
	public:
		static constexpr std::size_t size{20};
		typedef std::array<ClientCard::ID, size>::iterator Iterator;

		/// Copy constructor.
		Deck(const Deck& other) = default;

		/// Destructor.
		virtual ~Deck() = default;

		/// Default constructor.
		/// Creates a default deck. The default deck is the set of the \a size
		/// first cards (with the cards ordered by their ID).
		/// Without the default value for name, this would impossible to
        /// construct a `std::vector<Deck>` because std::vector requires that its
        /// elements are default-constructible. This is not the case if name
        /// has no default value.
		explicit Deck(const std::string& name = ""); // should take Deck id

		/// Constructor.
		/// Creates a deck from an array of cards.
		Deck(const std::string& name, const std::array<ClientCard::ID, size>& cards);

		ClientCard::ID getCard(std::size_t index) const;

		/// Replaces the card at \a index in the deck by \a card.
		void changeCard(std::size_t index, ClientCard::ID card);

		const std::string& getName() const;

		void setName(const std::string newName);

		Iterator begin();

		Iterator end();

	private:
		std::string _name;
		std::array<ClientCard::ID, size> _cards;  ///< All the cards of the deck.
};

#endif  // _DECK_COMMON_HPP
