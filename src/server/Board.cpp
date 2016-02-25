// WizardPoker headers
#include "server/Board.hpp"
// std-C++ headers
#include <random>

Board::Board(const PlayerInformations& player1, const PlayerInformations& player2)
{
	_activePlayer = new Player(player1.id, player1.socket, player1.specialSocket);
	_passivePlayer = new Player(player2.id, player2.socket, player2.specialSocket);
	// We make sure players know their opponents
	_activePlayer->setOpponent(_passivePlayer);
	_passivePlayer->setOpponent(_activePlayer);
	// A random player starts the game
	std::default_random_engine engine;
	if(std::bernoulli_distribution(0.5)(engine))
		std::swap(_activePlayer, _passivePlayer);

	_activePlayer->beginGame(true); //TODO: these have to request deck selection from player
	_passivePlayer->beginGame(false);
}

/*------------------------------ USER INTERFACE */
void Board::endTurn()
{
	_turn++; //turn counter (for both players)
	_activePlayer->leaveTurn();
	std::swap(_activePlayer, _passivePlayer); //swap active and inactive
	_activePlayer->enterTurn(_turn/2 +1); //ALWAYS call active player
}

void Board::useCard(int handIndex)
{
	//pass along to active player
	_activePlayer->useCard(handIndex);
}

void Board::attackWithCreature(int attackerIndex, int victimIndex)
{
	//pass along to active player
	_activePlayer->attackWithCreature(attackerIndex, victimIndex);
}

void Board::quitGame()
{
	//TODO: need identifier for player who quit
	//NETWORK: PLAYER_QUIT_GAME
}

Player::ID Board::getCurrentPlayerID()
{
	return _activePlayer->getID();
}

Player::ID Board::getWaitingPlayerID()
{
	return _passivePlayer->getID();
}

/*------------------------------ PLAYER AND CARD INTERFACE */
void Board::applyEffect(Card* usedCard, EffectParamsCollection effectArgs)
{
	int subject; //who the effect applies to
	try //check the input
	{
		subject=effectArgs.at(0);
		effectArgs.erase(effectArgs.begin());
	}
	catch (std::out_of_range)
	{
		//NETWORK: INPUT_ERROR
		return;
	}

	switch (subject)
	{
		case PLAYER_SELF:	//passive player
			_activePlayer->applyEffect(usedCard, effectArgs);
			break;

		case PLAYER_OPPO:	//active player
			_passivePlayer->applyEffect(usedCard, effectArgs);
			break;
		case CREATURE_SELF_THIS:	//active player's creature that was used
			{
				Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
				_activePlayer->applyEffectToCreature(usedCreature, effectArgs);
			}
			break;
		case CREATURE_SELF_INDX:	//active player's creature at given index
			//TODO: NETWORK : ASK_FOR_INDEX (_activePlayer)
			_activePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->requestSelfBoardIndex());
			break;

		case CREATURE_SELF_RAND:	//active player's creature at random index
			_activePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->getRandomBoardIndex());
			break;

		case CREATURE_SELF_TEAM:	//active player's team of creatures
			_activePlayer->applyEffectToCreatureTeam(usedCard, effectArgs);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			//TODO: NETWORK : ASK_FOR_INDEX (_activePlayer)
			_passivePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->requestOppoBoardIndex());
			break;

		case CREATURE_OPPO_RAND:	//passive player's creature at random index
			_passivePlayer->applyEffectToCreature(usedCard, effectArgs, _activePlayer->getRandomBoardIndex());
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			_passivePlayer->applyEffectToCreatureTeam(usedCard, effectArgs);
			break;
	}
	throw std::runtime_error("Effect subject not valid");
}

