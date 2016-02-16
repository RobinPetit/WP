#include <server/Board.hpp>

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

