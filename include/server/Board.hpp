#ifndef _BOARD_HPP
#define _BOARD_HPP

// WizardPoker headers
#include "server/Player.hpp"
#include "server/Card.hpp"
#include "server/Creature.hpp"
#include "server/Constraints.hpp"
#include "server/CardData.hpp"

/// Manages one game with two players
class Board
{
public:
	/// Constructor
	Board(Player::ID player1, Player::ID player2);

	/// Destructor
	~Board() = default;

	/// Interface for user
	void endTurn();
	void useCard(int handIndex);
	void attackWithCreature(int boardIndex, int victim);
	void quitGame(); //TODO: need identifier

	/// Interface for Player and Card classes
	void applyEffect(Card* usedCard, EffectParamsCollection effect);

private:
	int _turn = 0;
	Player *_activePlayer, *_passivePlayer;
};


#endif// _BOARD_HPP
