// WizardPoker headers
#include "server/Board.hpp"
// std-C++ headers
#include <random>

Board::Board(Player::ID player1, Player::ID player2)
{
	// A random player starts the game
	std::default_random_engine engine;
	std::uniform_int_distribution<unsigned> distribution(1, 2);
	if (distribution(engine) == 1)
		std::swap(player1, player2);
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

void Board::attackWithCreature(unsigned boardIndex, unsigned victim)
{
	_activePlayer->attackWithCreature(boardIndex, victim);
}

void Board::quitGame()
{
	//TODO: need identifier for player who quit
}

/*--------------------------- CARD INTERFACE */
void Board::applyEffect(Card* usedCard, EffectParamsCollection effectArgs)
{
	unsigned subject = effectArgs.front();
	effectArgs.erase(effectArgs.begin());
	unsigned method = effectArgs.front();
	effectArgs.erase(effectArgs.begin());

	switch (subject)
	{
		case PLAYER_SELF:
			(_activePlayer->*(_activePlayer->effectMethods[method]))(effectArgs);
			break;

		case PLAYER_OPPO:
			(_passivePlayer->*(_passivePlayer->effectMethods[method]))(effectArgs);
			break;

		case CREATURE_SELF:
			{
				Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
				(usedCreature->*(usedCreature->effectMethods[method]))(effectArgs);
			}
			break;

		case CREATURE_TEAM:
			_activePlayer->applyEffectToCreatures(method, effectArgs);
			break;

		case CREATURE_ONE_OPPO:
			{
				unsigned boardIndex = effectArgs.front();
				effectArgs.erase(effectArgs.begin());
				_passivePlayer->applyEffectToCreature(boardIndex, method, effectArgs);
			}
			break;

		case CREATURE_ALL_OPPO:
			_passivePlayer->applyEffectToCreatures(method, effectArgs);
			break;
	}
}

/*--------------------------- PLAYER INTERFACE */

