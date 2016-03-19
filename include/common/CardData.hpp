#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "common/Identifiers.hpp"
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS = 100;

///How to define an effect:
typedef std::vector<int> EffectParamsCollection;


/// Class containing the parameters defining an effect
/// In the following order:
/// EFFECT SUBJECT [+ SUBJECT INDEX] if subject is identified by index (INDX at the end)
/// EFFECT ID
/// if effect is SET_CONSTRAINT:
/// 	CONSTRAINT ID
/// 	followed by VALUE, TURNS (or UNLIMITED_TURNS), CONSTRAINT CONDITIONS
/// if effect is something else:
/// 	ALL ARGUMENTS (no more than 4)
class EffectArgs
{
private:
	const EffectParamsCollection _args;  ///< collection that defines the effect
	std::size_t _index;                  ///< index indicating where values should be read
public:
	/// Constructor
	/// \param effect a collection of effect parameters to copy
	EffectArgs(const EffectParamsCollection& effect);

	/// Constructor
	/// \param effect An explicit list of values describing the effect
	EffectArgs(const std::initializer_list<int>& effect);

	/// This function is used to get the first not used argument
	/// \return The current parameter
	int getArg();

	/// This function is used to check the first not used argument but
	/// to not signal to pass to the next one
	/// \return The current parameter
	int peekArg() const;

	/// This function is used to know how many parameters haven't been used yet
	/// \return the amount of not used parameters left in the collection
	int remainingArgs() const;

	/// This method is used to have a readable format of an object
	/// \return A string describing the parameters
	std::string toString() const;
};

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
