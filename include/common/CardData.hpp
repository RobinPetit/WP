#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <vector>
#include <sstream>
#include <memory>
#include "common/Identifiers.hpp" // CardId, DeckId...
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS = 100;

//TODO?: move to a server specific file?
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

// Common(Card|Creature|Spell)Data classes
/// CommonCardData class: hold data of a card template (i.e. a card out-game).
/// Common part to the client and server



class CommonCardData
{
protected:
	CardId _id;
	// use int instead of enum because
	// int is used in code
	// and static_cast from int to enum do not ensure that the value is valid
	// enum E{E0,E1}; E e(static_cast<E>(42)); //Code compiled without warning by gcc
	// so use ...Value enums add need of static_cast everywhere without any advantage.
	// The purpose of the enums in CardData.inc is (only) to have Cards.sql readable.
	// TODO? overload operator for enums to be able to use enums everywhere
	int _cost;

public:
	/// Constructor
	CommonCardData(CardId, int cost);

	/// Getters
	// I don't know why I get 'W used but never defined' and 'undefined reference to' when I inline?
	CardId getId() const;
	int getCost() const;

	/// Methods
	virtual bool isCreature() const = 0;
	virtual bool isSpell() const = 0;

	virtual ~CommonCardData() = default;
};


class CommonCreatureData : public CommonCardData
{
protected:
	int _attack;
	int _health;
	int _shield;
	int _shieldType;

public:
	/// Constructor
	CommonCreatureData(CardId, int cost,
	                   int attack, int health, int shield, int shieldType);

	/// Getters
	// I don't know why I get 'W used but never defined' and 'undefined reference to' when I inline?
	int getAttack() const;
	int getHealth() const;
	int getShield() const;
	int getShieldType() const;

	/// Methods
	virtual bool isCreature() const override;
	virtual bool isSpell() const override;

	virtual ~CommonCreatureData() = default;
};


class CommonSpellData : public CommonCardData
{
public:
	/// Constructor
	CommonSpellData(CardId, int cost);

	/// Methods
	virtual bool isCreature() const override;
	virtual bool isSpell() const override;

	virtual ~CommonSpellData() = default;
};

// forward declaration of classes actually used by server and client
//class ClientCreatureData;
//class ClientSpellData;
//class ServerCreatureData;
//class ServerSpellData;

#endif  // _CARD_DATA_HPP
