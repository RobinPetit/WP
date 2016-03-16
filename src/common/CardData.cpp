#include "common/CardData.hpp"

// CommonCardData base class
CommonCardData::CommonCardData(cardId id, int cost) :
	_id(id), _cost(cost)
{
}

cardId CommonCardData::getId() const
{
	return _id;
}

int CommonCardData::getCost() const
{
	return _cost;
}


// CommonCreatureData
CommonCreatureData::CommonCreatureData(cardId id, int cost,
                           int attack, int health, int shield, int shieldType) :
	CommonCardData(id,cost),
	_attack(attack), _health(health), _shield(shield), _shieldType(shieldType)
{
}

int CommonCreatureData::getAttack() const
{
	return _attack;
}

int CommonCreatureData::getHealth() const
{
	return _health;
}

int CommonCreatureData::getShield() const
{
	return _shield;
}

int CommonCreatureData::getShieldType() const
{
	return _shieldType;
}

bool CommonCreatureData::isCreature() const
{
	return true;
}

bool CommonCreatureData::isSpell() const
{
	return false;
}


// CommonSpellData
CommonSpellData::CommonSpellData(cardId id, int cost) :
	CommonCardData(id,cost)
{
}

bool CommonSpellData::isCreature() const
{
	return false;
}

bool CommonSpellData::isSpell() const
{
	return true;
}
