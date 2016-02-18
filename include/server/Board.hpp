#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <SFML/System.hpp>
#include <cstdlib>
#include "Player.hpp"
#include "server/Card.hpp"

/// Manages one game with two players
class Board
{
	public:
		/// Constructor
		Board(unsigned player1, unsigned player2);

		/// Destructor
		~Board() = default;

		/// Interface for user
		void endTurn();
		void useCard(unsigned handIndex);
		void attackWithCard(unsigned boardIndex, unsigned victim);
		void quitGame(); //TODO: need identifier

		/// Interface for Card class
		void applyEffect(unsigned player, unsigned effectID, std::vector<unsigned> effectArgs);

		/// Interface for Player class
		unsigned getDefaultConstraint(unsigned constraintID);

	private:
		unsigned _turn = 0;
		Player *_activePlayer, *_passivePlayer;

		//Effects
		void (Player::*_effects[3])(std::vector<unsigned>) =
		{
			&Player::setConstraint,
			//&Player::loseHandCards,
			&Player::damageBoardCreatures
		};

};


#endif// _BOARD_HPP
