#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>


/// WHO IS AFFECTED BY THE EFFECT ?
enum EFFECT_SUBJECTS : unsigned
{
    PLAYER_SELF,
    PLAYER_OPPO,
    CREATURE_SELF,
    CREATURE_TEAM,
    CREATURE_ONE_OPPO,
	CREATURE_ALL_OPPO
};

/// PLAYER EFFECTS
constexpr unsigned P_EFFECTS_COUNT = 11;
enum PLAYER_EFFECTS : unsigned
{
	//constraints
	PE_SET_CONSTRAINT,
	//other effects
	PE_PICK_DECK_CARDS,
	PE_LOSE_HAND_CARDS,
	PE_REVIVE_BIN_CARD,
	PE_STEAL_HAND_CARD,
	PE_EXCHG_HAND_CARD,
	PE_SET_ENERGY_POINTS,
	PE_ADD_ENERGY_POINTS,
	PE_SUB_ENERGY_POINTs,
	PE_ADD_HEALTH_POINTS,
	PE_SUB_HEALTH_POINTS
};

/// PLAYER CONSTRAINTS
constexpr unsigned P_CONSTRAINTS_COUNT = 10; //number of player constraints
enum PLAYER_CONSTRAINTS : unsigned
{
	//turn-by-turn
	PC_CARD_PICK_AMOUNT, 			//amount of cards to pick each turn
	PC_ENERGY_POINTS_INIT,			//initial amount of energy points each turn
	PC_HEALTH_POINTS_GAIN,			//amount of health points given each turn
	PC_HEALTH_POINTS_LOSS, 			//amount of health points taken each turn
	PC_HEALTH_POINTS_LOSS_DECK_EMPTY,	//amount of health points taken each turn when deck is empty
	//passive
	PC_USE_CARD_LIMIT,				//limit for using cards
	PC_CALL_SPELL_LIMIT,			//limit for calling spells
	PC_ATTACK_WITH_CREATURE_LIMIT,	//limit for attacking with creatures
	PC_PLACE_CREATURE_LIMIT,		//limit for placing creatures on board
	PC_CREATURES_ON_BOARD_LIMIT		//limit for number of creatures on the board
};

constexpr unsigned P_CONSTRAINT_DEFAULTS[P_CONSTRAINTS_COUNT] =
{
	//TURN-BASED CONSTRAINTS
	1,		//pick one card
	10,		//energy points start at 10
	0, 		//sub 0 health points
	0,		//add 0 health points
	5, 		//if deck empty, sub 5 health points
	//PASSIVE CONSTRAINTS
	100,	//no limit for using cards
	100,	//no limit for calling spells
	100, 	//no limit for attacking with creatures
	6,		//arbitrary limit for placing creatures (TODO: discuss)
	6		//arbitrary limit for amount of creatures on board
};

/// CREATURE EFFECTS
constexpr unsigned C_EFFECTS_COUNT = 11;
enum CREATURE_EFFECTS : unsigned
{
	//constraints
	CE_SET_CONSTRAINT,
	//other effects
	CE_RESET_ATTACK,
	CE_RESET_HEALTH,
	CE_RESET_SHIELD,
	CE_ADD_ATTACK,
	CE_ADD_HEALTH,
	CE_ADD_SHIELD,
	CE_SUB_ATTACK,
	CE_SUB_HEALTH,
	CE_SUB_SHIELD,
	CE_FORCED_SUB_HEALTH
};

/// CREATURE CONSTRAINTS
constexpr unsigned C_CONSTRAINTS_COUNT = 6; //number of creature constraints
enum CREATURE_CONSTRAINTS : unsigned
{
	//turn-by-turn
	CC_SELF_HEALTH_GAIN,			// Gain X health points
	CC_TEAM_HEALTH_GAIN,			// Team gains X health points
	CC_SELF_HEALTH_LOSS,
	CC_TEAM_HEALTH_LOSS,
	CC_SELF_ATTACK_GAIN,			// Gain X attack points
	CC_TEAM_ATTACK_GAIN,			// Team gains X attack points
	CC_SELF_ATTACK_LOSS,
	CC_TEAM_ATTACK_LOSS,
	CC_SELF_SHIELD_LOSS,			// Lose X shield points
	//passive
	CC_SELF_ATTACK_BLOCK,		// Block the next X attacks on himself
	CC_TEAM_ATTACK_BLOCK,		// Block the next X attacks on team
	CC_SELF_PARALYZED,			// Can not be used
	//ending (when creature dies)
    CC_END_TEAM_HEALTH_GAIN,
    CC_END_TEAM_ATTACK_LOSS,
    CC_END_TEAM_SHIELD_LOSS
	//TODO
	//CE_SELF_HYPNOTIZED,			// Can only be used by opponent
	//C_STICKY,			// Your opponent's spell cost X more next Turn
	//C_TRAP				// Deal 3 damage or summon two 1/1 snacks
};

constexpr unsigned C_CONSTRAINT_DEFAULTS[C_CONSTRAINTS_COUNT] =
{
	//TURN-BASED CONSTRAINTS: all set to 0, change with effects
	0,		//
	0,		//
	0, 		//
	0,		//
	0, 		//
	0		//
	//PASSIVE CONSTRAINTS: to come (agro, taunt...)
};

/// LA CLASSE!
class ConstraintList
{
private:
	const unsigned* _defaultValues;
	const unsigned _size;
	std::vector<std::pair<unsigned, unsigned>>* _timedValues;

public:
	ConstraintList(const unsigned* defaultValues, const unsigned arraySize);
	void setConstraint(unsigned constraintID, unsigned value, unsigned turns);
	unsigned getConstraint(unsigned constraintID);
	void timeOutConstraints();
};

#endif  // _CONSTRAINTS_HPP_
