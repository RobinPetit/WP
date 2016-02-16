#include <server/Player.hpp>

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

void Player::turnEnter(unsigned turn)
{
	//Communicate turn & as to play to menu
}

void Player::turnLeave(unsigned turn)
{
	//Communicate turn & ask to wait to menu

}

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
