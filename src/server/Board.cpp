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
void Board::applyEffect(Card* usedCard, std::vector<unsigned> effectArgs)
{
	const auto& effectIt = effectArgs.begin();
	unsigned subject = *effectIt;
	effectArgs.erase(effectIt);
	unsigned method = *effectIt;
	effectArgs.erase(effectIt);
	switch (subject)
	{
		case PLAYER_SELF:
			(_activePlayer->*(_activePlayer->effectMethods[method]))(effectArgs);
		case PLAYER_OPPO:
			(_passivePlayer->*(_passivePlayer->effectMethods[method]))(effectArgs);
		case CREATURE_SELF:
		{
			Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
			(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
		}
		case CREATURE_TEAM:
		{
            std::vector<Creature*> teamCreatures = _activePlayer->getBoardCreatures();
            for (unsigned i=0; i<teamCreatures.size(); i++)
            {
				Creature* usedCreature = teamCreatures.at(i);
				(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
            }
		}
		case CREATURE_ONE_OPPO:
		{
			unsigned boardIndex = *effectIt;
			effectArgs.erase(effectIt);
			Creature* usedCreature = _passivePlayer->getBoardCreatures().at(boardIndex);
			(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
		}
		case CREATURE_ALL_OPPO:
		{
			std::vector<Creature*> teamCreatures = _passivePlayer->getBoardCreatures();
            for (unsigned i=0; i<teamCreatures.size(); i++)
            {
				Creature* usedCreature = teamCreatures.at(i);
				(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
            }
		}
	}
}

/*--------------------------- PLAYER INTERFACE */

