#include "client/ClientCreature.hpp"

ClientCreature::ClientCreature(const CreatureData& cardData) :
	Card(cardData),
	_attack(cardData.getAttack()), _health(cardData.getHealth()), _shield(cardData.getShield()),
	_shieldType(cardData.getShieldType())
{
}

int ClientCreature::getAttack() const
{
	return _attack;
}

int ClientCreature::getHealth() const
{
	return _health;
}

int ClientCreature::getShield() const
{
	return _shield;
}

int ClientCreature::getShieldType() const
{
	return _shieldType;
}

const std::string& ClientCreature::getName() const
{
	return prototype().getName();
}

const std::string& ClientCreature::getDescription() const
{
	return prototype().getDescription();
}

/*--------------------------- PRIVATE METHOD */
inline const CreatureData& ClientCreature::prototype() const
{
	return static_cast<const CreatureData&>(_prototype);
}
