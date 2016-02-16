#include <server/Board.hpp>

void Board::endTurn()
{
	std::swap(_activePlayer, _passivePlayer);
	_turn++
	_passivePlayer.turnLeave(_turn/2 +1);
	_activePlayer.turnEnter(_turn/2 +1);
}

void Board::useCard(unsigned handIndex)
{
	_activePlayer.cardUse(handIndex);
}

void Board::quitGame()
{
	//TODO: need identifier for player who quit
}

