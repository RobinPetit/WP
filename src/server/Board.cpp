// WizardPoker headers
#include "server/Board.hpp"
// std-C++ headers
#include <random>

Board::Board(Player::ID player1, Player::ID player2, sf::TcpSocket& socketPlayer1, sf::TcpSocket& socketPlayer2)
{
	_activePlayer = new Player(player1, socketPlayer1);
	_passivePlayer = new Player(player2, socketPlayer2);
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

void Board::attackWithCreature(int boardIndex, int victim)
{
	//pass along to active player
	_activePlayer->attackWithCreature(boardIndex, victim);
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
	int subject = effectArgs.front(); //who the effect applies to
	effectArgs.erase(effectArgs.begin());

	int method = effectArgs.front(); //what method is used
	effectArgs.erase(effectArgs.begin());

	switch (subject)
	{
		case PLAYER_SELF:	//passive player
			_activePlayer->applyEffect(usedCard, method, effectArgs);
			break;

		case PLAYER_OPPO:	//active player
			_passivePlayer->applyEffect(usedCard, method, effectArgs);
			break;

		case CREATURE_SELF_THIS:	//active player's creature that was used
			{
				Creature* usedCreature = dynamic_cast<Creature*>(usedCard);
				_activePlayer->applyEffectToCreature(usedCreature, method, effectArgs);
			}
			break;
		case CREATURE_SELF_INDX:	//active player's creature at given index
            {
				int boardIndex = effectArgs.front();
				effectArgs.erase(effectArgs.begin());
				_activePlayer->applyEffectToCreature(usedCard, boardIndex, method, effectArgs);
            }
            break;

		case CREATURE_SELF_RAND:	//active player's creature at random index
            _activePlayer->applyEffectToCreature(usedCard, -1, method, effectArgs);
			break;

		case CREATURE_SELF_TEAM:	//active player's team of creatures
			_activePlayer->applyEffectToCreatures(usedCard, method, effectArgs);
			break;

		case CREATURE_OPPO_INDX:	//passive player's creature at given index
			{
				int boardIndex = effectArgs.front();
				effectArgs.erase(effectArgs.begin());
				_passivePlayer->applyEffectToCreature(usedCard, boardIndex, method, effectArgs);
			}
			break;

		case CREATURE_OPPO_RAND:    //passive player's creature at random index
			_passivePlayer->applyEffectToCreature(usedCard, -1, method, effectArgs);
			break;

		case CREATURE_OPPO_TEAM:	//passive player's team of creatures
			_passivePlayer->applyEffectToCreatures(usedCard, method, effectArgs);
			break;
	}
}

