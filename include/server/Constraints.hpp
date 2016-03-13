#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>
#include "common/CardData.hpp"

class Creature;

/// Constraint Rules
enum class ConstraintValueOption : int
{
	VALUE_FIXED,
	VALUE_GET_DECREMENT,
	VALUE_GET_INCREMENT,
	VALUE_TURN_DECREMENT,
	VALUE_TURN_INCREMENT
};

enum class ConstraintOrderOption : int
{
	GET_FIRST,
	GET_LAST,
	GET_SUM,
};

struct ConstraintDefaultValue
{
	int value;
	ConstraintValueOption valueOption;
	ConstraintOrderOption orderOption;
};

struct ConstraintTimedValue
{
	int value;
	int turns;
	const Creature* caster;
};

const std::vector<ConstraintDefaultValue> playerDefaultConstraints =
{
	//turn-based constraints
	{1, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},          //PC_TURN_CARDS_PICKED
	{1, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},          //PC_TURN_ENERGY_INIT_CHANGE
	{1, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //PC_TURN_ENERGY_CHANGE
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //PC_TURN_HEALTH_CHANGE
	{-5, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},          //PC_TURN_HEALTH_CHANGE_DECK_EMPTY
	//passive constraints
	{100, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},        //PC_TEMP_CARD_USE_LIMIT
	{100, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},        //PC_TEMP_SPELL_CALL_LIMIT
	{100, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},        //PC_TEMP_CREATURE_ATTACK_LIMIT
	{6, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},          //PC_TEMP_CREATURE_PLACING_LIMIT
	{6, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST}           //PC_TEMP_CREATURE_BOARD_LIMIT
};

const std::vector<ConstraintDefaultValue> creatureDefaultConstraints =
{
	//turn-based constraints: all default to 0
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //CC_TURN_ATTACK_CHANGE
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //CC_TURN_HEALTH_CHANGE
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //CC_TURN_SHIELD_CHANGE
	//passive
	{0, ConstraintValueOption::VALUE_GET_DECREMENT, ConstraintOrderOption::GET_LAST},  //CC_TEMP_FORCE_ATTACKS
	{0, ConstraintValueOption::VALUE_GET_DECREMENT, ConstraintOrderOption::GET_LAST},  //CC_TEMP_BLOCK_ATTACKS
	{0, ConstraintValueOption::VALUE_GET_DECREMENT, ConstraintOrderOption::GET_LAST},  //CC_TEMP_MIRROR_ATTACKS
	{0, ConstraintValueOption::VALUE_GET_DECREMENT, ConstraintOrderOption::GET_LAST},  //CC_TEMP_BACKFIRE_ATTACKS
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},          //CC_TEMP_DISABLE_ATTACKS
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_LAST},          //CC_TEMP_IS_PARALYZED
	//on creature death
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //CC_DEATH_TEAM_HEALTH_GAIN
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM},           //CC_DEATH_TEAM_ATTACK_LOSS
	{0, ConstraintValueOption::VALUE_FIXED, ConstraintOrderOption::GET_SUM}            //CC_DEATH_TEAM_SHIELD_LOSS
};

class Constraints
{
private:
	const std::vector<ConstraintDefaultValue>& _defaultValues;
	mutable std::vector<std::vector<ConstraintTimedValue>> _timedValues;

	int getValue(int constraintID, unsigned valueIndex) const;
	int getFirstTimedValue(int constraintID) const;
	int getLastTimedValue(int constraintID) const;
	int getSumTimedValues(int constraintID) const;

public:
	Constraints(const std::vector<ConstraintDefaultValue>& defaultValues);
	void setConstraint(int constraintID, int value, int turns, const Creature* caster=nullptr);
	int getConstraint(int constraintID) const;
	int getOverallConstraint(int constraintID, int otherValue) const;
	void timeOutConstraints();
};

#endif  // _CONSTRAINTS_HPP_
