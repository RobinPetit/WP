#include "server/Board.hpp"


Board::Board(unsigned player1, unsigned player2)
{
	// A random player starts the game
    if (rand() % 2 == 0) std::swap(player1, player2);
    _activePlayer = new Player(player1);
    _passivePlayer = new Player(player2);
    _activePlayer->setOpponent(_passivePlayer);
    _passivePlayer->setOpponent(_activePlayer);
}

/*--------------------------- USER INTERFACE */
void Board::endTurn()
{
	std::swap(_activePlayer, _passivePlayer);
	_turn++;
	_passivePlayer->leaveTurn(_turn/2 +1);
	_activePlayer->enterTurn(_turn/2 +1);
}

void Board::useCard(unsigned handIndex)
{
	_activePlayer->useCard(handIndex);
}

void Board::attackWithCard(unsigned boardIndex, unsigned victim)
{
	_activePlayer->attackWithCreature(boardIndex, victim);
}

void Board::quitGame()
{
	//TODO: need identifier for player who quit
}

/*--------------------------- CARD INTERFACE */
void Board::applyEffect(std::vector<unsigned> effect)
{
    //FIRST UNSIGNED: pointer to object
    //SECOND UNSIGNED: pointer to method
    //OTHER UNSIGNED: arguments
    //Player *effectSubject = (player==0 ? _activePlayer : _passivePlayer);
    // (effectSubject->(_effects[effectID-constraintsCount]*))(effectArgs);
}

/*--------------------------- PLAYER INTERFACE */

