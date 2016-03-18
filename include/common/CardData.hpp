#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "common/Identifiers.hpp"
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS=100;

///How to define an effect:
typedef std::vector<int> EffectParamsCollection;

class EffectArgs
{
private:
	const std::shared_ptr<const EffectParamsCollection> args; // pointer to parameters that define the effect
	std::size_t index; // index indicating where values should be read
public:
	EffectArgs(const EffectParamsCollection* effect): args(effect), index(0) {};
	EffectArgs(std::initializer_list<int> effect): args(new EffectParamsCollection(effect)), index(0) {};

	int getArg() {return args->at(index++);}
	int peekArg() {return args->at(index);}
	int remainingArgs() {return static_cast<int>(args->size()) - index;}
	std::string toString()
	{
		std::stringstream ss;
		ss << "[";
		for(size_t i = index; i < args->size(); ++i)
		{
			if(i != index)
				ss << ",";
			ss << args->at(i);
		}
		ss << "]";
		return ss.str();
	}
};
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
