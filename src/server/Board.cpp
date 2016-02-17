#include <server/Board.hpp>


/*--------------------------- USER INTERFACE */
void Board::endTurn()
{
	std::swap(_activePlayer, _passivePlayer);
	_turn++
	_passivePlayer.turnLeave(_turn/2 +1);
	_activePlayer.turnEnter(_turn/2 +1);
}

void Board::cardUse(unsigned handIndex)
{
	_activePlayer.cardUse(handIndex);
}

void Board::cardAttack(unsigned boardIndex, unsigned victim)
{
	_activePlayer.cardAttack(unsigned boardIndex, unsigned victim);
}

void Board::quitGame()
{
	//TODO: need identifier for player who quit
}

/*--------------------------- CARD INTERFACE */
void Board::applyEffect(unsigned player, unsigned effectID, std::vector<unsigned> effectArgs)
{
    Player* effectSubject = player==0 ? _activePlayer, _passivePlayer;
    (effectSubject->(_effects[effectID-constraintsCount]*))(effectArgs);
}

/*--------------------------- PLAYER INTERFACE */
unsigned Board::getDefaultConstraint(unsigned constraintID)
{
    return _defaultConstraints[constraintID];
}


