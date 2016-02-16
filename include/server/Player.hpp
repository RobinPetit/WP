#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <stack>
#include <SFML/System.hpp>
#include <Board.hpp>

//Forward declarations
class Card;

/// Represents one of the two players for a given game.
class Player
{
    public:
        /// Destructor.
        ~Player() = default;

        void cardPick(unsigned amount=0); // Move the card on top of the deck to the player's hand
        void cardUse(unsigned handIndex); // Use the card

        void cardDiscardFromHand(unsigned handIndex); // Move the card at handIndex from the player's hand to the bin
        void cardDiscardFromBoard(unsigned boardIndex); // Move the card at boardIndex from the board to the bin

    private:

        std::stack<std::unique_ptr<Card>> _cardDeck; ///< Cards that are in the deck (not usable yet)
        std::vector<std::unique_ptr<Card>> _cardHand; ///< Cards that are in the player's hand (usable)
        std::vector<std::unique_ptr<Card>> _cardBoard; ///< Cards that are on the board (usable for attacks)
        std::vector<std::unique_ptr<Card>> _cardBin; ///< Cards that are discarded (dead creatures, used spells)

        struct
        {
            std::vector<std::pair<unsigned, unsigned>> cardPickAmount = {{1, 0}};
            std::vector<std::pair<unsigned, unsigned>> handMaxSize = {{6, 0}};
        } _constraints;

        std::vector<std::vector<std::pair<unsigned, unsigned>> *> _constraintsVector;

        struct
        {
            unsigned cardsPicked=0;
            unsigned cardsUsed=0;
            unsigned creaturesPlaced=0;
            unsigned spellsUsed=0;
        } _turnData;
};


#endif// _PLAYER_HPP
