#include "server/Creature.hpp"

Creature::Creature(	unsigned cost, unsigned attack, unsigned health, unsigned shield, unsigned shieldType,
					std::vector<std::vector<unsigned>> effects):
	Card(cost, effects),
	_attack(attack), _health(health), _shield(shield), _shieldType(shieldType)
{

}

unsigned Creature::getAttack()
{
	return _attack;
}

unsigned Creature::getHealth()
{
	return _health;
}

bool Creature::isCreature()
{
	return true;
}

bool Creature::isSpell()
{
	return false;
}

/*--------------------------- EFFECTS */
void Creature::setConstraint(std::vector<unsigned> args)
{
	unsigned constraintID = args.at(0);
	unsigned value = args.at(1);
	unsigned turns = args.at(2);
	_constraints.setConstraint(constraintID, value, turns);
}

void Creature::resetAttack(std::vector<unsigned> args)
{
	 _attack = _attackInit;
}

void Creature::resetHealth(std::vector<unsigned> args)
{
	 _health = _healthInit;
}

void Creature::resetShield(std::vector<unsigned> args)
{
	 _shield = _shieldInit;
}

void Creature::addHealth(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_health += points;
}

void Creature::addAttack(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_attack += points;
}

void Creature::addShield(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_shield += points;
}

void Creature::subAttack(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	if(_attack > points) _attack -= points;
	else _attack = 0;
}

void Creature::subHealth(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	switch (_shieldType)
	{
		case 0:
			points-= _shield;	//Blue shield, can allow part of the attack to deal damage
		case 1:
			if (points <= _shield) points=0;	//Orange shield, only stronger attacks go through
		case 2:
			points=0;	//Legendary shield, regular attacks don't go through
	}

	if(_health > points) _health -= points;
	else
	{
		_health = 0;
		//TODO: creature_dead
	}
}

void Creature::forcedSubHealth(std::vector<unsigned> args)
{
	//Shields are not able to stop this attack
	unsigned points = args.at(0);
	if(_health > points) _health -= points;
	else
	{
		_health = 0;
		//TODO: creature_dead
	}
}

void Creature::subShield(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	if(_shield > points) _shield -= points;
	else _shield = 0;
}
