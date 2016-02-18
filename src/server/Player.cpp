#include "server/Player.hpp"
#include "server/Board.hpp"


void Player::Player(unsigned id):
	_id(id)
{

}


/*--------------------------- BOARD INTERFACE */
void Player::enterTurn(unsigned turn)
{
	_turnData = _emptyTurnData; //Clear the turn data
	pickCards(getConstraint(constraintIDs.cardPickAmount));
	setEnergyPoints(getConstraint(constraintIDs.startEnergyPoints));
	addLifePoints(getConstraint(constraintIDs.addLifePoints));
	subLifePoints(getConstraint(constraintIDs.subLifePoints));
	if (_cardDeck.empty()) subLifePoints(getConstraint(constraintIDs.emptyDeckSubLifePoitns));
}

void Player::leaveTurn(unsigned turn)
{
	timeOutConstraints();
	//Communicate turn & ask to wait to menu
}

void Player::pickCards(unsigned amount)
{
	while (not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.top());
		_cardDeck.pop();
		_turnData.cardsPicked++;
	}
}

void Player::useCard(unsigned handIndex)
{
	// TODO: verify that handIndex is not out_of_range
	const auto& handIt = std::find(_cardHand.begin(), _cardHand.end(), _cardHand[handIndex]); //Iterator

    if (getConstraint(constraintIDs.useCardLimit) == _turnData.cardsUsed)
    {
        //Throw some kind of error back to user
        return;
    }
	_turnData.cardsUsed++;

	// If card is a creature
	if (_cardHand.at(handIndex).isCreature())
	{
		if (getConstraint(constraintIDs.placeCreatureLimit) == _turnData.creaturesPlaced)
		{
            //Throw some kind of error back to user
            return;
		}
		_turnData.creaturesPlaced++;
		_cardBoard.push_back(_cardHand.at(handIndex));
		_cardHand.erase(handIt);
	}
	// If card is a spell
	else
	{
		if (getConstraint(constraintIDs.callSpellLimit) == _turnData.spellCalls)
		{
			//Throw some kind of error back to user
			return;
		}
		_turnData.spellCalls++;
		cardDiscardFromHand(handIndex);
	}
}

void Player::attackWithCreature(unsigned boardIndex, unsigned victim)
{
	if (getConstraint(constraintIDs.attackWithCreatureLimit) == _turnData.creatureAttacks)
	{
		//Throw some kind of error back to user
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

void Player::addLifePoints(unsigned points)
{
    _lifePoints += points;
}

void Player::subLifePoints(unsigned points)
{
    if (_lifePoints > points) _lifePoints -= points
    else
    {
		_lifePoints = 0;
		//DEAD
    }
}


/*--------------------------- PRIVATE */
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

