#include "server/Creature.hpp"

std::function<void(Creature&, const EffectParamsCollection&)> Creature::effectMethods[P_EFFECTS_COUNT] =
{
	&Creature::setConstraint,
	&Creature::resetAttack,
	&Creature::resetHealth,
	&Creature::resetShield,
	&Creature::changeAttack,
	&Creature::changeHealth,
	&Creature::changeShield
};

Creature::Creature(int cost, int attack, int health, int shield, int shieldType,
					std::vector<EffectParamsCollection> effects):
	Card(cost, effects),
	_attack(attack),
	_health(health),
	_shield(shield),
	_shieldType(shieldType)
{

}

int Creature::getAttack()
{
	return _attack;
}

int Creature::getHealth()
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

int Creature::getConstraint(int constraintID)
{
    return _constraints.getConstraint(constraintID);
}


/*--------------------------- PLAYER INTERFACE */
void Creature::enterTurn()
{
	//Creature's turn-based rules
	changeHealth({_constraints.getConstraint(CC_TURN_SELF_HEALTH_CHANGE)});
	_owner->applyEffectToCreatures(this, CE_CHANGE_HEALTH, {_constraints.getConstraint(CC_TURN_TEAM_HEALTH_CHANGE),1});

	changeAttack({_constraints.getConstraint(CC_TURN_SELF_ATTACK_CHANGE)});
	_owner->applyEffectToCreatures(this, CE_CHANGE_ATTACK, {_constraints.getConstraint(CC_TURN_TEAM_ATTACK_CHANGE)});

	changeShield({_constraints.getConstraint(CC_TURN_SELF_SHIELD_CHANGE)});
	_owner->applyEffectToCreatures(this, CE_CHANGE_SHIELD, {_constraints.getConstraint(CC_TURN_TEAM_SHIELD_CHANGE)});
}

void Creature::leaveTurn()
{
    _constraints.timeOutConstraints();
}

/*--------------------------- EFFECTS */
void Creature::setConstraint(const EffectParamsCollection& args)
{
	int constraintID = args.at(0);
	int value = args.at(1);
	int turns = args.at(2);
	if (args.size()>3)
		_constraints.setConstraint(constraintID, value, turns);
	else
		_constraints.setConstraint(constraintID, value, turns, dynamic_cast<const Creature*>(_owner->getLastCaster()));
}

void Creature::resetAttack(const EffectParamsCollection&)
{
	 _attack = _attackInit;
}

void Creature::resetHealth(const EffectParamsCollection&)
{
	 _health = _healthInit;
}

void Creature::resetShield(const EffectParamsCollection&)
{
	 _shield = _shieldInit;
}

void Creature::changeAttack(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_attack+=points;
	if (_attack<0) _attack=0;
}

void Creature::changeHealth(const EffectParamsCollection& args)
{
	int points = args.at(0);
	//bool forced = args.at(1);
    if (points<0 and (args.size()==1 or args.at(1)==0))
    {
		switch (_shieldType)
		{
			case 0:
				points+= _shield;	//Blue shield, can allow part of the attack to deal damage
				if (points>0) points=0;
				break;
			case 1:
				if (points <= _shield) points=0;	//Orange shield, only stronger attacks go through
				break;
			case 2:
				points=0;	//Legendary shield, regular attacks don't go through
				break;
		}
    }

	_health+=points;
	if (_health<=0)
	{
		_health=0;
		//Creature died
	}
}

void Creature::changeShield(const EffectParamsCollection& args)
{
	int points = args.at(0);
	_shield += points;
	if (_shield<0) _shield=0;
}
