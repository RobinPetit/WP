#include <server/Player.hpp>

void Player::cardPick(unsigned amount)
{
    while (not _cardDeck.empty() and amount>0)
    {
        amount--;
        _cardHand.push_back(_cardDeck.pop());
    }
}

void Player::cardPlaceOnBoard(int handIndex)
{
    _cardBoard.push_back(_cardHand.at(handIndex));
    _cardHand.erase(handIndex);
}

void Player::cardDiscardFromHand(int handIndex)
{
    _cardBin.push_back(_cardHand.at(handIndex));
    _cardHand.erase(handIndex);
}

void Player::cardDiscardFromBoard(int boardIndex)
{
    _cardBin.push_back(_cardBoard.at(boardIndex));
    _cardBoard.erase(boardIndex);
}
