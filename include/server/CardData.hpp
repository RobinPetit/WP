#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>

/// Constraint Rules
enum CONSTRAINT_VALUE_RULE : int
{
    VALUE_FIXED,
    VALUE_GET_DECREMENT,
    VALUE_GET_INCREMENT,
    VALUE_TURN_DECREMENT,
    VALUE_TURN_INCREMENT
};

/// PLAYER EFFECTS
enum PLAYER_EFFECTS : int
{
	//constraints
	PE_SET_CONSTRAINT,
	//other effects
	PE_PICK_DECK_CARDS,
	PE_LOSE_HAND_CARDS,
	PE_REVIVE_BIN_CARD,
	PE_STEAL_HAND_CARD,
	PE_EXCHG_HAND_CARD,
	PE_SET_ENERGY,
	PE_CHANGE_ENERGY,
	PE_CHANGE_HEALTH,
	//count
	P_EFFECTS_COUNT
};

/// PLAYER CONSTRAINTS
enum PLAYER_CONSTRAINTS : int
{
	//turn-by-turn
	PC_TURN_CARDS_PICKED, 			//amount of cards to pick each turn
	PC_TURN_ENERGY_INIT,			//initial amount of energy points each turn
	PC_TURN_ENERGY_CHANGE,			//amount of energy points added each turn
	PC_TURN_HEALTH_CHANGE,			//amount of health points added each turn
	PC_TURN_HEALTH_CHANGE_DECK_EMPTY,	//amount of health points taken each turn when deck is empty
	//passive
	PC_LIMIT_CARD_USE,				//limit for using cards
	PC_LIMIT_SPELL_CALL,			//limit for calling spells
	PC_LIMIT_CREATURE_ATTACK,		//limit for attacking with creatures
	PC_LIMIT_CREATURE_PLACING,		//limit for placing creatures on board
	PC_LIMIT_CREATURE_BOARD,		//limit for number of creatures on the board
	//count
	P_CONSTRAINTS_COUNT
};

constexpr std::pair<int, CONSTRAINT_VALUE_RULE> P_CONSTRAINT_DEFAULTS[P_CONSTRAINTS_COUNT] =
{
	//TURN-BASED CONSTRAINTS
	{1,VALUE_FIXED},		//PC_TURN_CARDS_PICKED
	{10,VALUE_FIXED},		//PC_TURN_ENERGY_INIT
	{10,VALUE_FIXED},		//PC_TURN_ENERGY_CHANGE
	{0,VALUE_FIXED}, 		//PC_TURN_HEALTH_CHANGE
	{5,VALUE_FIXED}, 		//PC_TURN_HEALTH_CHANGE_DECK_EMPTY
	//PASSIVE CONSTRAINTS
	{100,VALUE_FIXED},		//PC_LIMIT_CARD_USE
	{100,VALUE_FIXED},		//PC_LIMIT_SPELL_CALL
	{100,VALUE_FIXED}, 		//PC_LIMIT_CREATURE_ATTACK
	{6,VALUE_FIXED},		//PC_LIMIT_CREATURE_PLACING
	{6,VALUE_FIXED}			//PC_LIMIT_CREATURE_BOARD
};

/// CREATURE EFFECTS
enum CREATURE_EFFECTS : int
{
	//constraints
	CE_SET_CONSTRAINT,
	//other effects
	CE_RESET_ATTACK,
	CE_RESET_HEALTH,
	CE_RESET_SHIELD,
	CE_CHANGE_ATTACK,
	CE_CHANGE_HEALTH,
	CE_CHANGE_SHIELD,
	//count
	C_EFFECTS_COUNT
};

/// CREATURE CONSTRAINTS
enum CREATURE_CONSTRAINTS : int
{
	//turn-by-turn
	CC_TURN_SELF_ATTACK_CHANGE,			//Attack points added each turn
	CC_TURN_SELF_HEALTH_CHANGE,			//Health points added each turn
	CC_TURN_SELF_SHIELD_CHANGE,			//Shield points added each turn
    CC_TURN_TEAM_ATTACK_CHANGE,
    CC_TURN_TEAM_HEALTH_CHANGE,
    CC_TURN_TEAM_SHIELD_CHANGE,
	//passive
	CC_SELF_BLOCK_ATTACKS,			// Block the next X attacks on himself
	CC_TEAM_BLOCK_ATTACKS,			// Block the next X attacks on team
	CC_SELF_PARALYZED,				// Can not be used
	//on creature death
	CC_DEATH_TEAM_ATTACK_CHANGE,
	CC_DEATH_TEAM_HEALTH_CHANGE,
	CC_DEATH_TEAM_SHIELD_CHANGE,
	//count
	C_CONSTRAINTS_COUNT
};

constexpr std::pair<int, CONSTRAINT_VALUE_RULE> C_CONSTRAINT_DEFAULTS[C_CONSTRAINTS_COUNT] =
{
	//turn-by-turn: all default to 0
	{0,VALUE_FIXED},		//CC_TURN_ATTACK_CHANGE
	{0,VALUE_FIXED},		//CC_TURN_HEALTH_CHANGE
	{0,VALUE_FIXED}, 		//CC_TURN_SHIELD_CHANGE
	{0,VALUE_FIXED},		//CC_TURN_TEAM_ATTACK_CHANGE
	{0,VALUE_FIXED}, 		//CC_TURN_TEAM_HEALTH_CHANGE
	{0,VALUE_FIXED},		//CC_TURN_TEAM_SHIELD_CHANGE
	//passive
	{0,VALUE_GET_DECREMENT},	//CC_SELF_BLOCK_ATTACKS
	{0,VALUE_GET_DECREMENT},	//CC_TEAM_BLOCK_ATTACKS
	{0,VALUE_FIXED},			//CC_SELF_PARALYZED
	//on creature death
	{0,VALUE_FIXED},		//CC_END_TEAM_HEALTH_GAIN
	{0,VALUE_FIXED},		//CC_END_TEAM_ATTACK_LOSS
	{0,VALUE_FIXED}			//CC_END_TEAM_SHIELD_LOSS
};


///Used types
enum CostValue : int
{
	COST_0,COST_1,COST_2,COST_3,COST_4,COST_5,COST_6,COST_7,COST_8,COST_9,COST_10
};

enum AttackValue : int
{
	ATTACK_0,ATTACK_1,ATTACK_2,ATTACK_3,ATTACK_4,ATTACK_5,ATTACK_6,ATTACK_7,ATTACK_8,ATTACK_9,ATTACK_10
};

enum HealthValue : int
{
	HEALTH_1,HEALTH_2,HEALTH_3,HEALTH_4,HEALTH_5,HEALTH_6,HEALTH_7,HEALTH_8,HEALTH_9,
	HEALTH_10,HEALTH_11,HEALTH_12,HEALTH_13,HEALTH_14,HEALTH_15,HEALTH_16,HEALTH_17,HEALTH_18,HEALTH_19,HEALTH_20
};

enum ShieldValue: int
{
	SHIELD_0,SHIELD_1,SHIELD_2,SHIELD_3,SHIELD_4,SHIELD_5
};

enum ShieldType : int
{
	SHIELD_NONE,		//no shield
	SHIELD_BLUE,		//allows part of the attack to deal damage if attack is stronger than shield
	SHIELD_ORANGE,		//allows all the attack to deal damage if attack is stronger than shield
	SHIELD_LEGENDARY	//creature attacks don't go through
};

typedef std::vector<int> EffectParamsCollection;

enum EFFECT_SUBJECTS : int
{
	PLAYER_SELF,
	PLAYER_OPPO,

	CREATURE_SELF_THIS,
	CREATURE_SELF_INDX,
	CREATURE_SELF_RAND,
	CREATURE_SELF_TEAM,

	CREATURE_OPPO_INDX,
	CREATURE_OPPO_RAND,
	CREATURE_OPPO_TEAM
};

///Creature type
struct CreatureData
{
	std::string name;
	CostValue cost;
	AttackValue attack;
	HealthValue health;
	ShieldValue shield;
	ShieldType shieldType;
	std::vector<EffectParamsCollection> effects;
	std::string description;
};

///Spell type
struct SpellData
{
	std::string name;
	CostValue cost;
	std::vector<EffectParamsCollection> effects;
	std::string description;
};

extern const CreatureData ALL_CREATURES[];

extern const SpellData ALL_SPELLS[];

#endif  // _CARD_DATA_HPP
