#include "server/Player.hpp"
#include "server/Board.hpp"


void Player::Player(unsigned id, Player* opponent):
	_id(id)
{
	//NETWORK: GREETINGS_USER
}

void Player::setOpponent(Player* opponent)
{
	_opponent = opponent;
}


/*--------------------------- BOARD INTERFACE */
void Player::enterTurn(unsigned turn)
{
	//Player's turn-based rules
	_turnData = _emptyTurnData; //Clear the turn data
	pickCards(getConstraint(constraintIDs.cardPickAmount));
	setEnergyPoints(getConstraint(constraintIDs.startEnergyPoints));
	addLifePoints(getConstraint(constraintIDs.addLifePoints));
	subLifePoints(getConstraint(constraintIDs.subLifePoints));
	if (_cardDeck.empty()) subLifePoints(getConstraint(constraintIDs.emptyDeckSubLifePoitns));
	//Call creature's turn-based rules
	//NETWORK: TURN_STARTED
}

void Player::leaveTurn(unsigned turn)
{
	timeOutConstraints();
	//NETWORK: TURN_ENDED
}

void Player::useCard(unsigned handIndex)
{
	//TODO: verify that handIndex is not out_of_range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]); //Iterator

    if (getConstraint(constraintIDs.useCardLimit) == _turnData.cardsUsed)
    {
        //NETWORK: USE_CARDS_LIMIT
        return;
    }

	// If card is a creature
	if (_cardHand.at(handIndex).isCreature())
	{
		if (getConstraint(constraintIDs.placeCreatureLimit) == _turnData.creaturesPlaced)
		{
            //NETWORK: PLACE_CREATURES_LIMIT
            return;
		}
		_turnData.cardsUsed++;
		_turnData.creaturesPlaced++;
		_cardBoard.push_back(_cardHand.at(handIndex));
		_cardHand.erase(handIt);
		//NETWORK: BOARD_CHANGED
		//NETWORK: CREATURE_PLACED
	}

	// If card is a spell
	else
	{
		if (getConstraint(constraintIDs.callSpellLimit) == _turnData.spellCalls)
		{
			//NETWORK: CALL_SPELLS_LIMIT
			return;
		}
		_turnData.cardsUsed++;
		_turnData.spellCalls++;
		cardDiscardFromHand(handIndex);
		//NETWORK: BOARD_CHANGED
		//NETWORK: SPELL_CALLED
	}
}

void Player::attackWithCreature(unsigned boardIndex, unsigned victim)
{
	if (getConstraint(constraintIDs.attackWithCreatureLimit) == _turnData.creatureAttacks)
	{
		//NETWORK: CREATURE_ATTACKS_LIMIT
		return;
	}
    //attack with _cardBoard.at(boardIndex) against victim
}

/*--------------------------- CONSTRAINTS */
void Player::setConstraint(unsigned constraintID, unsigned value, unsigned turns)
{
	_constraintsArray[constraintID].push_back(std::make_pair(value, turns));
}

unsigned Player::getConstraint(unsigned constraintID)
{
    if (_constraintsArray[constraintID].empty())
	return _constraintDefaults[constraintID];
    else
	return _constraintsArray[constraintID].rbegin()->first;
}

void Player::timeOutConstraints()
{
    for (int i=0; i<_constraintsArray.size(); i++)
    {
        std::vector<std::pair<unsigned, unsigned>> vect = _constraintsArray.at(i);
        for (std::vector<std::pair<unsigned, unsigned>>::iterator vectIt=vect.begin(); vectIt!=vect.end();)
        {
            if (*vectIt.second == 1) vectIt = vect.erase(vectIt)
            else
            {
				vectIt->second--;
				vectIt++;
            }
        }
    }
}

/*--------------------------- EFFECTS */
void Player::pickSomeCards(std::vector<unsigned> args)
{
    pickCards(args.at(0));
}

void Player::loseHandCards(std::vector<unsigned> args)
{
	unsigned amount = args.at(1);
	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		unsigned handIndex = rand() % _cardHand.size();
		cardDiscardFromHand(handIndex);
	}
}

void stealHandCard(std::vector<unsigned> args)
{
	_cardHand.push_back(_opponent.cardRemoveFromHand());
}

void exchangeHandCard(std::vector<unsigned> args)
{
	unsigned myCardIndex = args.at(0);
	Card* myCard = _cardHand.at(myCardIndex);

	_cardHand.at(myCardIndex) = _opponent.cardExchangeFromHand(myCard);
	if (_cardHand.at(myCardIndex) == nullptr)
	{
		const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
		_cardHand.erase(handIt);
	}
}

void Player::addEnergyPoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_energyPoints += points;
}

void subEnergyPoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
    if (_energyPoints > points) _energyPoints -= points
    else
    {
		_energyPoints = 0;
		//NETWORK NO_MORE_ENERGY
		//call endTurn()
    }
}

void Player::addLifePoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
    _lifePoints += points;
}

void Player::subLifePoints(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
    if (_lifePoints > points) _lifePoints -= points
    else
    {
		_lifePoints = 0;
		//call die()
    }
}


/*--------------------------- PRIVATE */
void Player::pickCards(unsigned amount)
{
	if(not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.top());
		_cardDeck.pop();
		_turnData.cardsPicked++;
	}
}

void Player::cardDiscardFromHand(unsigned handIndex)
{
	// TODO: treat properly case of handIndex being out of range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIt);
}

void Player::cardDiscardFromBoard(unsigned boardIndex)
{
	const auto& boardIt = std::find(_cardBoard.begin(), _cardBoard.end(), _cardBoard[boardIndex]);
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIt);
}

Card* Player::cardRemoveFromHand()
{
	if (_cardHand.empty()) return nullptr;

	unsigned handIndex = rand() % _cardHand.size();
	Card* stolen = _cardHand[handIndex];
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]);
	_cardHand.erase(handIt);
	return stolen;
}

Card* Player::cardExchangeFromHand(Card* given)
{
    if (_cardHand.empty()) return nullptr;

    unsigned handIndex = rand() % _cardHand.size();
	Card* stolen = _cardHand[handIndex];
    _cardHand.at(handIndex) = given;
    return stolen;
}
