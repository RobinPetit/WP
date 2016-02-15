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

    private:

        std::stack<std::unique_ptr<Card>> _CardStack;///< Stack of cards that are to be picked (not usable yet)
};


#endif// _PLAYER_HPP
