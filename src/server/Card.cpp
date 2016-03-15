#include "server/Card.hpp"

Card::Card(const CommonCardData& cardData):
	_prototype(cardData), _cost(cardData.getCost())
{
}

int Card::getEnergyCost() const
{
	return _cost;
}

cardId Card::getID() const
{
	return _prototype.getId();
}

bool Card::isCreature() const
{
	return _prototype.isCreature();
}

bool Card::isSpell() const
{
	return _prototype.isSpell();
}
