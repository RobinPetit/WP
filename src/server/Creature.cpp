// WizardPoker headers
#include "server/Player.hpp"
#include "server/Creature.hpp"
// std-C++ headers
#include <iostream>
#include <cassert>

std::array<std::function<void(Creature&, EffectArgs)>, P_EFFECTS_COUNT> Creature::_effectMethods =
{
	&Creature::setConstraint,
	&Creature::resetAttack,
	&Creature::resetHealth,
	&Creature::resetShield,
	&Creature::changeAttack,
	&Creature::changeHealth,
	&Creature::changeShield
};

Creature::Creature(cardId cardIdentifier, Player& owner, int cost, int attack, int health, int shield, int shieldType,
		std::vector<EffectParamsCollection> effects):
	Card(cardIdentifier, cost, effects),
	_attack(attack),
	_health(health),
	_shield(shield),
	_shieldType(shieldType),
	_owner(owner)
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

void Creature::moveToBoard()
{
	_isOnBoard = true;
}

void Creature::removeFromBoard()
{
	_isOnBoard = false;
	//Creature's death-based constraints
	changeAttack({getConstraint(CC_DEATH_ATTACK_CHANGE)});
	changeHealth({getConstraint(CC_DEATH_HEALTH_CHANGE)});
	changeShield({getConstraint(CC_DEATH_ATTACK_CHANGE)});
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
	if(isParalyzed == 1) //Creature can not be used
		return;

	int attackDisabled = getConstraint(CC_TEMP_DISABLE_ATTACKS);
	if(attackDisabled == 1) //Creature can not attack
		return;

	int attackForced = getConstraint(CC_TEMP_FORCE_ATTACKS);

	int attackBackfires = getConstraint(CC_TEMP_BACKFIRE_ATTACKS);
	if(attackBackfires == 1)	//Attack turns agains the creature
		changeHealth({_attack, attackForced});
	else
		victim.receiveAttack(*this, _attack, attackForced);
}

void Creature::receiveAttack(Creature& attacker, int attack, int forced, int loopCount)
{
	if(loopCount >= 2) //If both creatures mirror attacks, no one is damaged
		return;

	int attackMirrored = getConstraint(CC_TEMP_MIRROR_ATTACKS);
	if(attackMirrored == 1) //If attacks are mirrored, we send it back
		attacker.receiveAttack(*this, attack, forced, loopCount+1);

	bool attackBlocked = getConstraint(CC_TEMP_BLOCK_ATTACKS) != 0;
	if(not attackBlocked)  // Only attack if attacks are not blocked
		changeHealth({-attack, forced});
}

/*--------------------------- GETTERS FOR EFFECTS */
void Creature::applyEffectToSelf(EffectArgs effect)
{
	assert(effect.remainingArgs() >= 1);
	const int method = effect.getArg();  // What method is used
	_effectMethods.at(method)(*this, effect);  // Call the method
}

int Creature::getAttack() const
{
	return _attack;
}

int Creature::getHealth() const
{
	return _health;
}

int Creature::getShield() const
{
	return _shield;
}

int Creature::getShieldType() const
{
	return _shieldType;
}

int Creature::getPersonalConstraint(int constraintId) const
{
	return _constraints.getConstraint(constraintId);
}

int Creature::getConstraint(int constraintId) const
{
	return _owner.getCreatureConstraint(*this, constraintId);
}

/*--------------------------- EFFECTS */
void Creature::setConstraint(EffectArgs effect)
{
	int constraintId;  // constraint to set
	int value;  // value to give to it
	int turns;  // for how many turns
	int casterOptions;  // whether the constraint depends on its caster being alive
	try  // check the input
	{
		constraintId = effect.getArg();
		value = effect.getArg();
		turns = effect.getArg();
		casterOptions = effect.getArg();
		if(constraintId < 0 or constraintId >= C_CONSTRAINTS_COUNT or turns < 0)
			throw std::out_of_range("");
	}
	catch(std::out_of_range&)
	{
		 throw std::runtime_error("Creature::setConstraint error with cards arguments");
	}

	switch(casterOptions)
	{
		case IF_CASTER_ALIVE:
			_constraints.setConstraint(constraintId, value, turns, dynamic_cast<const Creature*>(_owner.getLastCaster()));
			break;

		default:
			_constraints.setConstraint(constraintId, value, turns);
			break;
	}
}

void Creature::resetAttack(EffectArgs /* effect */)
{
	// no arguments
	 _attack = _attackInit;
}

void Creature::resetHealth(EffectArgs /* effect */)
{
	// no arguments
	 _health = _healthInit;
}

void Creature::resetShield(EffectArgs /* effect */)
{
	// no arguments
	 _shield = _shieldInit;
}

void Creature::changeAttack(EffectArgs effect)
{
	try //check the input
	{
		_attack += effect.getArg();
		if(_attack < 0)
			_attack = 0;
	}
	catch (std::out_of_range&)
	{
		throw std::runtime_error("changeAttack error with cards arguments");
	}
}

void Creature::changeHealth(EffectArgs effect)
{
	int points;  // health points to add
	bool forced;
	try  // check the input
	{
		points = effect.getArg();
		if (effect.remainingArgs() >= 1)
			forced = effect.getArg() != 0;
		else
			forced = false;
	}
	catch (std::out_of_range&)
	{
		 throw std::runtime_error("changeHealth error with cards arguments");
	}

	if(points < 0 and not forced)
	{
		switch (_shieldType)
		{
			case SHIELD_BLUE:
				points += _shield;  // Blue shield, can allow part of the attack to deal damage
				if(points > 0)
					points = 0;
				break;
			case SHIELD_ORANGE:
				if(-points <= _shield)
					points = 0;  // Orange shield, only stronger attacks go through
				break;
			case SHIELD_LEGENDARY:
				points = 0;  // Legendary shield, regular attacks don't go through
				break;
		}
	}

	_health += points;
	if(_health <= 0)
	{
		_health = 0;
		if(_isOnBoard)
			_owner.cardBoardToGraveyard(this);
	}
}

void Creature::changeShield(EffectArgs effect)
{
	try  // check the input
	{
		_shield += effect.getArg();
		if(_shield < 0)
			_shield = 0;
	}
	catch (std::out_of_range&)
	{
		 throw std::runtime_error("changeShield error with cards arguments");
	}
}

Creature::operator BoardCreatureData() const
{
	BoardCreatureData data {getId(), getHealth(), getAttack(), getShield(),
			BoardCreatureData::shieldTypes[getShieldType()]};
	return data;
}
