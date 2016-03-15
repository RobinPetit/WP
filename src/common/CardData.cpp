#include "common/CardData.hpp"

// CommonCardData base class
CommonCardData::CommonCardData(cardId id, CostValue cost) :
	_id(id), _cost(cost)
{
}

cardId CommonCardData::getId() const
{
	return _id;
}

CostValue CommonCardData::getCost() const
{
	return _cost;
}


// CommonCreatureData
CommonCreatureData::CommonCreatureData(cardId id, CostValue cost,
                           AttackValue attack, HealthValue health, ShieldValue shield, ShieldType shieldType) :
	CommonCardData(id,cost),
	_attack(attack), _health(health), _shield(shield), _shieldType(shieldType)
{
}

AttackValue CommonCreatureData::getAttack() const
{
	return _attack;
}

HealthValue CommonCreatureData::getHealth() const
{
	return _health;
}

ShieldValue CommonCreatureData::getShield() const
{
	return _shield;
}

ShieldType CommonCreatureData::getShieldType() const
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
CommonSpellData::CommonSpellData(cardId id, CostValue cost) :
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
