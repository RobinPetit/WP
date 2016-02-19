#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <SFML/System.hpp>
#include <cstdlib>
#include "Player.hpp"
#include "Card.hpp"
#include "Creature.hpp"
#include "Constraints.hpp"

/// Manages one game with two players
class Board
{
public:
	/// Constructor
	Board(unsigned int player1, unsigned int player2);

	/// Destructor
	~Board() = default;

	/// Interface for user
	void endTurn();
	void useCard(unsigned handIndex);
	void attackWithCreature(unsigned boardIndex, unsigned victim);
	void quitGame(); //TODO: need identifier

	/// Interface for Player and Card classes
	void applyEffect(Card* usedCard, std::vector<unsigned> effect);

private:
	unsigned _turn = 0;
	Player *_activePlayer, *_passivePlayer;
};


#endif// _BOARD_HPP
