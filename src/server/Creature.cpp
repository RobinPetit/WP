// WizardPoker headers
#include "server/Creature.hpp"
// std-C++ headers
#include <iostream>

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

Creature::Creature(const ServerCreatureData& cardData, Player& owner):
	Card(cardData),
	_attack(cardData.getAttack()),
	_health(cardData.getHealth()),
	_shield(cardData.getShield()),
	_shieldType(cardData.getShieldType()),
	_owner(owner)
{
}

void Creature::moveToBoard()
{
	_isOnBoard = true;
}

void Creature::removeFromBoard()
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

/*--------------------------- EFFECTS INTERFACE */
void Creature::applyEffectToSelf(EffectParamsCollection& effectArgs)
{
	int method = effectArgs.front(); //what method is used
	effectArgs.erase(effectArgs.begin());

	_effectMethods[method](*this, effectArgs);
}

std::vector<EffectParamsCollection> Creature::getEffects()
{
	return prototype().getEffects();
}

int Creature::getAttack()
{
	return _attack;
}

int Creature::getHealth()
{
	return _health;
}

int Creature::getShield()
{
	return _shield;
}

int Creature::getShieldType()
{
	return _shieldType;
}

int Creature::getPersonalConstraint(int constraintID) const
{
	return _constraints.getConstraint(constraintID);
}

int Creature::getConstraint(int constraintID) const
{
	return _owner.getCreatureConstraint(*this, constraintID);
}

/*--------------------------- EFFECTS */
void Creature::setConstraint(const EffectParamsCollection& args)
{
	int constraintID; //constraint to set
	int value; //value to give to it
	int turns; //for how many turns
	int casterOptions; //whether the constraint depends on its caster being alive
	try //check the input
	{
		constraintID=args.at(0);
		value=args.at(1);
		turns=args.at(2);
		casterOptions=args.at(3);
		if (constraintID<0 or constraintID>=C_CONSTRAINTS_COUNT or turns<0)
			throw std::out_of_range("");
	}
	catch (std::out_of_range&)
	{
		 throw std::runtime_error("Error with cards arguments");
	}

	switch (casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintID, value, turns, dynamic_cast<const Creature*>(_owner.getLastCaster()));
			break;
		default:
			_constraints.setConstraint(constraintID, value, turns);
			break;
	}
}

void Creature::resetAttack(const EffectParamsCollection&)
{
	//no arguments
	 _attack = prototype().getAttack();
}

void Creature::resetHealth(const EffectParamsCollection&)
{
	//arguments
	 _health = prototype().getHealth();
}

void Creature::resetShield(const EffectParamsCollection&)
{
	//no arguments
	 _shield = prototype().getShield();
}

void Creature::changeAttack(const EffectParamsCollection& args)
{
	try //check the input
	{
		_attack += args.at(0);
		if(_attack < 0)
			_attack = 0;
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("Error with cards arguments");
	}
}

void Creature::changeHealth(const EffectParamsCollection& args)
{
	int points; //health points to add
	try //check the input
	{
		points=args.at(0);
	}
	catch (std::out_of_range&)
	{
		 throw std::runtime_error("Error with cards arguments");
	}

	//bool forced = args.at(1) : if attack is forced, shield does not count
	if (points<0 and (args.size()==1 or args.at(1)==0))
	{
		switch (_shieldType)
		{
			case SHIELD_BLUE:
				points+= _shield;	//Blue shield, can allow part of the attack to deal damage
				if (points>0)
					points=0;
				break;
			case SHIELD_ORANGE:
				if (points <= _shield)
					points=0;	//Orange shield, only stronger attacks go through
				break;
			case SHIELD_LEGENDARY:
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
	try //check the input
	{
		_shield += args.at(0);
		if(_shield < 0)
			_shield = 0;
	}
	catch (std::out_of_range&)
	{
		 throw std::runtime_error("Error with cards arguments");
	}
}

/*--------------------------- PRIVATE METHOD */
inline const ServerCreatureData& Creature::prototype() const
{
	return static_cast<const ServerCreatureData&>(_prototype);
}
