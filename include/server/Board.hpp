#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <stack>
#include <SFML/System.hpp>
#include <Player.hpp>

//Forward declarations
class Card;

/// Manages one game with two players
class Board
{
    public:
        /// Destructor.
        ~Board() = default;

    private:

        std::pair<Player*> _Players;///< Pair of players that play the game
};


#endif// _BOARD_HPP
