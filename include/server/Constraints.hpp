#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>
#include "server/CardData.hpp"

class Creature;

/// Constraint Rules
enum CONSTRAINT_VALUE_OPTIONS : int
{
	VALUE_FIXED,
	VALUE_GET_DECREMENT,
	VALUE_GET_INCREMENT,
	VALUE_TURN_DECREMENT,
	VALUE_TURN_INCREMENT
};

enum CONSTRAINT_GET_OPTIONS : int
{
	GET_FIRST,
    GET_LAST,
    GET_SUM,
};

struct ConstraintDefaultValue
{
    int value;
    CONSTRAINT_VALUE_OPTIONS valueOption;
    CONSTRAINT_GET_OPTIONS getOption;
};

struct ConstraintTimedValue
{
    int value;
    int turns;
    const Creature* caster;
};

constexpr ConstraintDefaultValue P_CONSTRAINT_DEFAULTS[P_CONSTRAINTS_COUNT] =
{
	//TURN-BASED CONSTRAINTS
	{1,VALUE_FIXED,GET_LAST},		//PC_TURN_CARDS_PICKED
	{10,VALUE_FIXED,GET_LAST},		//PC_TURN_ENERGY_INIT
	{10,VALUE_FIXED,GET_SUM},		//PC_TURN_ENERGY_CHANGE
	{0,VALUE_FIXED,GET_SUM}, 		//PC_TURN_HEALTH_CHANGE
	{5,VALUE_FIXED,GET_LAST}, 		//PC_TURN_HEALTH_CHANGE_DECK_EMPTY
	//PASSIVE CONSTRAINTS
	{100,VALUE_FIXED,GET_LAST},		//PC_LIMIT_CARD_USE
	{100,VALUE_FIXED,GET_LAST},		//PC_LIMIT_SPELL_CALL
	{100,VALUE_FIXED,GET_LAST}, 	//PC_LIMIT_CREATURE_ATTACK
	{6,VALUE_FIXED,GET_LAST},		//PC_LIMIT_CREATURE_PLACING
	{6,VALUE_FIXED,GET_LAST}		//PC_LIMIT_CREATURE_BOARD
};

constexpr ConstraintDefaultValue C_CONSTRAINT_DEFAULTS[C_CONSTRAINTS_COUNT] =
{
	//turn-by-turn: all default to 0
	{0,VALUE_FIXED,GET_SUM},		//CC_TURN_SELF_ATTACK_CHANGE
	{0,VALUE_FIXED,GET_SUM},		//CC_TURN_SELF_HEALTH_CHANGE
	{0,VALUE_FIXED,GET_SUM}, 		//CC_TURN_SELF_SHIELD_CHANGE
	{0,VALUE_FIXED,GET_SUM},		//CC_TURN_TEAM_ATTACK_CHANGE
	{0,VALUE_FIXED,GET_SUM}, 		//CC_TURN_TEAM_HEALTH_CHANGE
	{0,VALUE_FIXED,GET_SUM},		//CC_TURN_TEAM_SHIELD_CHANGE
	//passive
	{0,VALUE_GET_DECREMENT,GET_FIRST},	//CC_SELF_BLOCK_ATTACKS
	{0,VALUE_GET_DECREMENT,GET_FIRST},	//CC_TEAM_BLOCK_ATTACKS
	{0,VALUE_FIXED,GET_LAST},			//CC_SELF_PARALYZED
	//on creature death
	{0,VALUE_FIXED,GET_SUM},		//CC_END_TEAM_HEALTH_GAIN
	{0,VALUE_FIXED,GET_SUM},		//CC_END_TEAM_ATTACK_LOSS
	{0,VALUE_FIXED,GET_SUM}			//CC_END_TEAM_SHIELD_LOSS
};

class Constraints
{
private:
	const unsigned _size;
	const ConstraintDefaultValue* _defaultValues;
	std::vector<ConstraintTimedValue>* _timedValues;
	int getTimedValue(int constraintID, unsigned valueIndex);

public:
	Constraints(const ConstraintDefaultValue* defaultValues, const int arraySize);
	void setConstraint(int constraintID, int value, int turns);
	void setConstraint(int constraintID, int value, int turns, const Creature* caster);
	int getConstraint(int constraintID);
	void timeOutConstraints();
};

#endif  // _CONSTRAINTS_HPP_
