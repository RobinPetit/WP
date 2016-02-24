#include "server/Creature.hpp"

std::function<void(Creature&, const EffectParamsCollection&)> Creature::_effectMethods[P_EFFECTS_COUNT] =
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

bool Creature::isCreature()
{
	return true;
}

bool Creature::isSpell()
{
	return false;
}

void Creature::movedToBoard()
{
	_isOnBoard = true;
}

void Creature::removedFromBoard()
{
	//Creature's death-based constraints
	changeAttack({getConstraint(CC_DEATH_ATTACK_CHANGE)});
	changeHealth({getConstraint(CC_DEATH_HEALTH_CHANGE)});
	changeShield({getConstraint(CC_DEATH_ATTACK_CHANGE)});
	_isOnBoard = false;
}

bool Creature::isOnBoard() const
{
	return _isOnBoard;
}


/*--------------------------- PLAYER INTERFACE */
void Creature::enterTurn()
{
	//Creature's turn-based constraints
	changeAttack({getConstraint(CC_TURN_ATTACK_CHANGE)});
	changeHealth({getConstraint(CC_TURN_HEALTH_CHANGE)});
	changeShield({getConstraint(CC_TURN_ATTACK_CHANGE)});
}

void Creature::leaveTurn()
{
	_constraints.timeOutConstraints();
}

void Creature::makeAttack(Creature& victim)
{
	int isParalyzed = getConstraint(CC_TEMP_IS_PARALYZED);
	if (isParalyzed==1) //Creature can not be used
		return;

	int attackDisabled = getConstraint(CC_TEMP_DISABLE_ATTACKS);
	if (attackDisabled==1) //Creature can not attack
		return;

	int attackForced = getConstraint(CC_TEMP_FORCE_ATTACKS);

	int attackBackfires = getConstraint(CC_TEMP_BACKFIRE_ATTACKS);
	if (attackBackfires==1)	//Attack turns agains the creature
		changeHealth({_attack, attackForced});
	else
		victim.receiveAttack(*this, _attack, attackForced);
}

void Creature::receiveAttack(Creature& attacker, int attack, int forced, int loopCount)
{
	if (loopCount>=2) //If both creatures mirror attacks, no one is damaged
		return;

	int attackMirrored = getConstraint(CC_TEMP_MIRROR_ATTACKS);
	if (attackMirrored==1) //If attacks are mirrored, we send it back
		attacker.receiveAttack(*this, attack, forced, loopCount+1);

	int attackBlocked = getConstraint(CC_TEMP_BLOCK_ATTACKS);
	if (attackBlocked==1) //If attacks are blocked
		return;

	changeHealth({attack, forced});
}

/*--------------------------- GETTERS FOR EFFECTS */
void Creature::applyEffect(EffectParamsCollection effectArgs)
{
	int method = effectArgs.front(); //what method is used
	effectArgs.erase(effectArgs.begin());

	_effectMethods[method](*this, effectArgs);
}

int Creature::getAttack()
{
	return _attack;
}

int Creature::getPersonalConstraint(int constraintID) const
{
	return _constraints.getConstraint(constraintID);
}

int Creature::getConstraint(int constraintID) const
{
	return _owner->getCreatureConstraint(*this, constraintID);
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

	//bool forced = args.at(1) : if attack is forced, shield does not count
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
