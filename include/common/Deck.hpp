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
// \TODO Card.hpp should implements a type Card::ID
// (maybe such as `typedef std::size_t Card::ID`)
class Deck
{
    public:
        static constexpr std::size_t size{20};
        typedef std::array<Card::ID, size>::iterator Iterator;

        /// Copy constructor.
        Deck(const Deck& other) = default;

        /// Destructor.
        virtual ~Deck() = default;

        /// Constructor.
        /// Creates a default deck. The default deck is the set of the \a size
        /// first cards (with the cards ordered by their ID).
        explicit Deck(const std::string& name);

        /// Constructor.
        /// Creates a deck from an array of cards.
        Deck(const std::string& name, const std::array<Card::ID, size>& cards);

        Card::ID getCard(std::size_t index) const;

        /// Replaces the card at \a index in the deck by \a card.
        void changeCard(std::size_t index, Card::ID card);

        const std::string& getName() const;

        void setName(const std::string newName);

        Iterator begin();

        Iterator end();

    private:
        std::string _name;
        std::array<Card::ID, size> _cards;  ///< All the cards of the deck.
};

#endif  // _DECK_COMMON_HPP
