#include "client/ClientCreature.hpp"

ClientCreature::ClientCreature(cardId cardIdentifier, const std::string& name, int cost,
                               const std::string& description, int attack, int health, int shield, int shieldType) :
	ClientCard(cardIdentifier, name, cost, description),
	_attack(attack), _health(health), _shield(shield), _shieldType(shieldType)
{

}

bool ClientCreature::isCreature()
{
	return true;
}

bool ClientCreature::isSpell()
{
	return false;
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
