#include "server/Creature.hpp"

Creature::Creature(unsigned cost, unsigned attack, unsigned health, unsigned shield, unsigned shieldType,
					std::vector<std::vector<unsigned>> effects):
	Card(cost, effects),
	_attack(attack),
	_health(health),
	_shield(shield),
	_shieldType(shieldType)
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

unsigned Creature::getConstraint(unsigned constraintID)
{
    return _constraints.getConstraint(constraintID);
}


/*--------------------------- PLAYER INTERFACE */
void Creature::enterTurn()
{
	//Creature's turn-based rules
	addHealth({_constraints.getConstraint(CC_SELF_HEALTH_GAIN)});
	_owner->applyEffectToCreatures(CE_ADD_HEALTH, {_constraints.getConstraint(CC_TEAM_HEALTH_GAIN)});
	forcedSubHealth({_constraints.getConstraint(CC_SELF_HEALTH_LOSS)});
	_owner->applyEffectToCreatures(CE_FORCED_SUB_HEALTH, {_constraints.getConstraint(CC_TEAM_HEALTH_LOSS)});
	addAttack({_constraints.getConstraint(CC_SELF_ATTACK_GAIN)});
	_owner->applyEffectToCreatures(CE_ADD_ATTACK, {_constraints.getConstraint(CC_TEAM_ATTACK_GAIN)});
	subAttack({_constraints.getConstraint(CC_SELF_ATTACK_LOSS)});
	_owner->applyEffectToCreatures(CE_SUB_ATTACK, {_constraints.getConstraint(CC_TEAM_ATTACK_LOSS)});
	subShield({_constraints.getConstraint(CC_SELF_SHIELD_LOSS)});
}

void Creature::leaveTurn()
{
    _constraints.timeOutConstraints();
}

/*--------------------------- EFFECTS */
void Creature::setConstraint(const std::vector<unsigned>& args)
{
	unsigned constraintID = args.at(0);
	unsigned value = args.at(1);
	unsigned turns = args.at(2);
	_constraints.setConstraint(constraintID, value, turns);
}

void Creature::resetAttack(const std::vector<unsigned>&)
{
	 _attack = _attackInit;
}

void Creature::resetHealth(const std::vector<unsigned>&)
{
	 _health = _healthInit;
}

void Creature::resetShield(const std::vector<unsigned>&)
{
	 _shield = _shieldInit;
}

void Creature::addHealth(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_health += points;
}

void Creature::addAttack(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_attack += points;
}

void Creature::addShield(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	_shield += points;
}

void Creature::subAttack(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	if(_attack > points) _attack -= points;
	else _attack = 0;
}

void Creature::subHealth(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	switch (_shieldType)
	{
		case 0:
			points-= _shield;	//Blue shield, can allow part of the attack to deal damage
			break;
		case 1:
			if (points <= _shield) points=0;	//Orange shield, only stronger attacks go through
			break;
		case 2:
			points=0;	//Legendary shield, regular attacks don't go through
			break;
	}

	if(_health > points) _health -= points;
	else
	{
		_health = 0;
		//TODO: creature_dead
	}
}

void Creature::forcedSubHealth(const std::vector<unsigned>& args)
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

void Creature::subShield(const std::vector<unsigned>& args)
{
	unsigned points = args.at(0);
	if(_shield > points) _shield -= points;
	else _shield = 0;
}
