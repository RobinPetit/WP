#include <server/Player.hpp>


/*--------------------------- BOARD INTERFACE */
void Player::cardPick(unsigned amount)
{
	while (not _cardDeck.empty() and amount>0)
	{
		amount--;
		_cardHand.push_back(_cardDeck.pop());
		_turnData.cardsPicked++;
	}
}

void Player::cardUse(unsigned handIndex)
{
	_turnData.cardsUsed++;
	// If card is a creature
	if (true)
	{
		_turnData.creaturesPlaced++;
		_cardBoard.push_back(_cardHand.at(handIndex));
		_cardHand.erase(handIndex);
	}
	// If card is a spell
	else
	{
		_turnData.spellsUsed++;
		cardDiscardFromHand(handIndex);
	}
}

void Player::cardAttack(unsigned boardIndex, unsigned victim)
{
    //attack with _cardBoard.at(boardIndex) against victim
}

void Player::turnEnter(unsigned turn)
{
	//Communicate turn & as to play to menu
}

void Player::turnLeave(unsigned turn)
{
	//Communicate turn & ask to wait to menu
}


/*--------------------------- CONSTRAINTS */
void Player::setConstraint(unsigned constraintID, unsigned value, unsigned turns)
{
	_constraintsArray[constraintID].push_back(std::make_pair(value, turns));
}

unsigned Player::getConstraint(unsigned constraintID)
{
    if (_constraintsArray[constraintID].empty()) return _board.getDefaultConstraint(constraintID);
    else return _constraintsArray[constraintID].rbegin().first();
}


/*--------------------------- EFFECTS */
void Player::loseHandCards(unsigned amount)
{
	while (not _cardHand.empty() and amount>0)
	{
		amount--;
		unsigned handIndex = rand() % _cardHand.size();
		cardDiscardFromHand(handIndex);
	}
}


/*--------------------------- PRIVATE */
void Player::cardDiscardFromHand(unsigned handIndex)
{
	_cardBin.push_back(_cardHand.at(handIndex));
	_cardHand.erase(handIndex);
}

void Player::cardDiscardFromBoard(unsigned boardIndex)
{
	_cardBin.push_back(_cardBoard.at(boardIndex));
	_cardBoard.erase(boardIndex);
}

