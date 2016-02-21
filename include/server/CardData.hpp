#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include "server/Constraints.hpp"


/// PLAYER EFFECTS
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
	PE_SUB_HEALTH_POINTS,
	//count
	P_EFFECTS_COUNT
};

/// PLAYER CONSTRAINTS
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
	PC_CREATURES_ON_BOARD_LIMIT,	//limit for number of creatures on the board
	//count
	P_CONSTRAINTS_COUNT
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
	CE_FORCED_SUB_HEALTH,
	//count
	C_EFFECTS_COUNT
};

/// CREATURE CONSTRAINTS
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
	CC_SELF_BLOCK_ATTACKS,		// Block the next X attacks on himself
	CC_TEAM_BLOCK_ATTACKS,		// Block the next X attacks on team
	CC_SELF_PARALYZED,			// Can not be used
	//ending (when creature dies)
	CC_END_TEAM_HEALTH_GAIN,
	CC_END_TEAM_ATTACK_LOSS,
	CC_END_TEAM_SHIELD_LOSS,
	//count
	C_CONSTRAINTS_COUNT
	//TODO
	//CE_SELF_HYPNOTIZED,			// Can only be used by opponent
	//C_STICKY,			// Your opponent's spell cost X more next Turn
	//C_TRAP				// Deal 3 damage or summon two 1/1 snacks
};

constexpr unsigned C_CONSTRAINT_DEFAULTS[C_CONSTRAINTS_COUNT] =
{
	//turn-by-turn: all default to 0
	0,		//
	0,		//
	0, 		//
	0,		//
	0, 		//
	0,		//
	0,		//
	0,		//
	0,		//
	//passive
	0,
	0,
	0,
	//ending
	0,
	0,
	0
};


///Used types
enum CostValue : unsigned
{
	COST_0,COST_1,COST_2,COST_3,COST_4,COST_5,COST_6,COST_7,COST_8,COST_9,COST_10
};

enum AttackValue : unsigned
{
	ATTACK_0,ATTACK_1,ATTACK_2,ATTACK_3,ATTACK_4,ATTACK_5,ATTACK_6,ATTACK_7,ATTACK_8,ATTACK_9,ATTACK_10
};

enum HealthValue : unsigned
{
	HEALTH_1,HEALTH_2,HEALTH_3,HEALTH_4,HEALTH_5,HEALTH_6,HEALTH_7,HEALTH_8,HEALTH_9,
	HEALTH_10,HEALTH_11,HEALTH_12,HEALTH_13,HEALTH_14,HEALTH_15,HEALTH_16,HEALTH_17,HEALTH_18,HEALTH_19,HEALTH_20
};

enum ShieldValue: unsigned
{
	SHIELD_1,SHIELD_2,SHIELD_3,SHIELD_4,SHIELD_5
};

enum ShieldType : unsigned
{
	SHIELD_NONE,		//no shield
	SHIELD_BLUE,		//allows part of the attack to deal damage if attack is stronger than shield
	SHIELD_ORANGE,		//allows all the attack to deal damage if attack is stronger than shield
	SHIELD_LEGENDARY	//creature attacks don't go through
};

typedef std::vector<unsigned> EffectParamsCollection;

enum EFFECT_SUBJECTS : unsigned
{
	PLAYER_SELF,
	PLAYER_OPPO,
	CREATURE_SELF,
	CREATURE_TEAM,
	CREATURE_ONE_OPPO,
	CREATURE_ALL_OPPO
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

///CREATURES
CreatureData ALL_CREATURES[] = {
	{
		"Stanislas",
		COST_2, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},		{{}},
		"No effect"
	{
        	"Allan",
		COST_10, ATTACK_9, HEALTH_9, SHIELD_5, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
        	"Theo",
		COST_2, ATTACK_3, HEALTH_2, SHIELD_3, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
        	"Robin",
		COST_5, ATTACK_4, HEALTH_6, SHIELD_4, SHIELD_ORANGE,
		{{}},
		"No effect"
	},
	{
        	"Nicolas",
		COST_7, ATTACK_7, HEALTH_7, SHIELD_1, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
 		"Alexis",
		COST_2, ATTACK_2, HEALTH_8, SHIELD_2, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
        	"Yves",
		COST_10, ATTACK_10, HEALTH_10, SHIELD_5, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
        	"Nikita",
		COST_6, ATTACK_7, HEALTH_5, SHIELD_1, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
        	"Keno",
		COST_1, ATTACK_1, HEALTH_1, SHIELD_1, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
        	"Ragnaros",
		COST_8, ATTACK_8, HEALTH_8, SHIELD_5, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
        	"Thierry",
		COST_1, ATTACK_0, HEALTH_1, SHIELD_1, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
        	"Tannenbaum",
		COST_4, ATTACK_4, HEALTH_5, SHIELD_5, SHIELD_ORANGE,
		{{}},
		"No effect"
	}
};

///SPELLS
SpellData ALL_SPELLS[] = {
	{
        "Abracadabra",
        COST_10,
        {{PLAYER_OPPO, PE_LOSE_HAND_CARDS, 2}},
		"Your opponent loses 2 cards from his hand"
	},
	{
		"Block",
		COST_9,
		{{CREATURE_TEAM, CC_TEAM_BLOCK_ATTACKS, 3}},
		"Block the next 3 attacks"
	},
	{
		"Care",
		COST_5,
		{{CREATURE_SELF, CE_ADD_HEALTH, 2}},
		"Increase a monster's health of 2 points"
	},
	{
		"Exchange",
		COST_7,
		{{PLAYER_OPPO, PE_EXCHG_HAND_CARD, 1}},
		"Each player chose an opponent's card and trade them"
	},
	{
		"Hypnosis",
		COST_4,
		{{CREATURE_ONE_OPPO, CE_SELF_HYPNOTIZED, 3}},
		"Take control of an ennemy creature for 3 turn"
	},
	{
		"Mirror",
		COST_6,
		{{CREATURE_ONE_OPPO, 0/*Effect missing*/, 0}},
		"Send back the next opponent's attack"
	},
	{
		"Paralyze",
		COST_3,
		{{CREATURE_ONE_OPPO, CC_SELF_BLOCK_ATTACKS, 2}},
		"Forbid the opponent to use a card for 2 turns"
	},
	{
		"Resurrection",
		COST_9,
		{{CREATURE_SELF, PE_REVIVE_BIN_CARD, 1}},
		"Allow the player to get 1 discarded card"
	},
	{
		"Shield",
		COST_2,
		{{CREATURE_TEAM, CC_SELF_BLOCK_ATTACKS, 1}},
		"Absord the next opponent's attack"
	},
	{
		"Stealing",
		COST_8,
		{{PLAYER_OPPO, PE_STEAL_HAND_CARD, 1}},
		"Take 1 random card from the opponent's deck"
	},
	{
		"Sticky",
		COST_7,	//~I put a random cost, as it was not specified in CSV file (NBA)
		{{CREATURE_ALL_OPPO, C_STICKY, 2}},
		"Your opponent's spell cost 2 more next Turn"
	},
	{
		"Trap",
		COST_2,
		{{CREATURE_ONE_OPPO, C_TRAP, 3}},
		"Deal 3 damage or summon two 1/1 snacks"
	},
	{
		"Metamorphosis",
		COST_4,
		{{CREATURE_SELF, 0/*Effect missing*/, 1}},
		"Transforms 1 creature in another random creature"
	}
};

#endif // _CARD_DATA_HPP
