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

		void endTurn();
		void useCard(unsigned handIndex);
		void quitGame(); //TODO: need identifier


	private:
		unsigned _turn = 0;
		Player* _activePlayer, _passivePlayer;
};


#endif// _BOARD_HPP
