#ifndef _CLIENT_CREATURE_CLIENT_HPP
#define _CLIENT_CREATURE_CLIENT_HPP

#include "common/Card.hpp"
#include "client/CardData.hpp"

///Spell card : One of the 2 playables card
class ClientCreature: public Card
{
private:
	int _attack, _health, _shield;
	int _shieldType;

	/// used to get good prototype type
	inline const CreatureData& prototype() const;

public:
	/// Constructor
	ClientCreature(const CreatureData&);

	// GETTERS
	const std::string& getName() const;
	const std::string& getDescription() const;
	int getAttack() const;
	int getHealth() const;
	int getShield() const;
	int getShieldType() const;

	// TODO: client too should store when values change to be used by gui from here
	// SETTERS
};

#endif // _CLIENT_CREATURE_CLIENT_HPP
