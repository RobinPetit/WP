#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>
#include "common/Identifiers.hpp"
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS = 100;

/// Player constraints
enum PLAYER_CONSTRAINTS : int32_t
{
  //turn-by-turn
  PC_TURN_CARDS_PICKED = 0,          //amount of cards to pick each turn
  PC_TURN_ENERGY_INIT_CHANGE,        //amount of points added to init value each turn
  PC_TURN_ENERGY_CHANGE,             //amount of energy points added each turn
  PC_TURN_HEALTH_CHANGE,             //amount of health points added each turn
  PC_TURN_HEALTH_CHANGE_DECK_EMPTY,  //amount of health points taken each turn when deck is empty
  //passive
  PC_TEMP_CARD_USE_LIMIT,            //limit for using cards
  PC_TEMP_SPELL_CALL_LIMIT,          //limit for calling spells
  PC_TEMP_CREATURE_ATTACK_LIMIT,     //limit for attacking with creatures
  PC_TEMP_CREATURE_PLACING_LIMIT,    //limit for placing creatures on board
  PC_TEMP_CREATURE_BOARD_LIMIT,      //limit for number of creatures on the board
  //count
  P_CONSTRAINTS_COUNT
};

///How to define an effect:
typedef std::vector<int> EffectParamsCollection;
//In the following order:
//EFFECT SUBJECT [+ SUBJECT INDEX] if subject is identified by index (INDX at the end)
//EFFECT ID
//if effect is SET_CONSTRAINT:
//	CONSTRAINT ID
//	followed by VALUE, TURNS (or UNLIMITED_TURNS), CONSTRAINT CONDITIONS
//if effect is something else:
//	ALL ARGUMENTS (no more than 4)

///Creature struct
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

///Spell struct
struct SpellData
{
	std::string name;
	CostValue cost;
	std::vector<EffectParamsCollection> effects;
	std::string description;
};

extern const CreatureData ALL_CREATURES[];

extern const SpellData ALL_SPELLS[];

extern const std::size_t nbSpells;

extern const std::size_t nbCreatures;

#endif  // _CARD_DATA_HPP
