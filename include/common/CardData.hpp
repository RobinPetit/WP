#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>
#include "common/Identifiers.hpp"


#include "CardData.inc"

constexpr int UNLIMITED_TURNS=100;

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

#endif  // _CARD_DATA_HPP
