#ifndef _CARD_DATA_HPP_
#define _CARD_DATA_HPP_

#include <vector>
#include "common/Identifiers.hpp" // cardId, deckId...
#include "common/CardData.inc"

constexpr int UNLIMITED_TURNS = 100;

//TODO: move to appropriate file
/// Player constraints
enum PLAYER_CONSTRAINTS : int32_t
{
    //turn-by-turn
    PC_TURN_CARDS_PICKED = 1,          //amount of cards to pick each turn
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

//TODO?: move to a server specific file?
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

// Common(Card|Creature|Spell)Data classes
/// CommonCardData class: hold data of a card template (i.e. a card out-game).
/// Common part to the client and server



class CommonCardData
{
protected:
	cardId _id;
	CostValue _cost;

public:
	/// Constructor
	CommonCardData(cardId, CostValue);

	/// Getters
	inline cardId getId() const;
	inline CostValue getCost() const;

	/// Methods
	virtual bool isCreature() const = 0;
	virtual bool isSpell() const = 0;

	virtual ~CommonCardData() = default;
};


class CommonCreatureData : public CommonCardData
{
protected:
	AttackValue _attack;
	HealthValue _health;
	ShieldValue _shield;
	ShieldType _shieldType;

public:
	/// Constructor
	CommonCreatureData(cardId, CostValue,
	                   AttackValue, HealthValue, ShieldValue, ShieldType);

	/// Getters
	inline AttackValue getAttack() const;
	inline HealthValue getHealth() const;
	inline ShieldValue getShield() const;
	inline ShieldType getShieldType() const;

	/// Methods
	virtual bool isCreature() const override;
	virtual bool isSpell() const override;

	virtual ~CommonCreatureData() = default;
};


class CommonSpellData : public CommonCardData
{
public:
	/// Constructor
	CommonSpellData(cardId, CostValue);

	/// Methods
	virtual bool isCreature() const override;
	virtual bool isSpell() const override;

	virtual ~CommonSpellData() = default;
};

// forward declaration of classes actually used by server and client
//class CreatureData;
//class SpellData;

#endif  // _CARD_DATA_HPP
