#ifndef _CLIENT_CREATURE_CLIENT_HPP
#define _CLIENT_CREATURE_CLIENT_HPP

#include "client/ClientCard.hpp"

///Spell card : One of the 2 playables card
class ClientCreature: public ClientCard
{
private:
	int _attack, _health, _shield;
	int _shieldType;

public:
	/// Constructor
	ClientCreature(cardId cardIdentifier, const std::string& name, int cost, const std::string& description,
	               int attack, int health, int shield, int shieldType);

	// GETTERS
	int getAttack() const;
	int getHealth() const;
	int getShield() const;
	int getShieldType() const;

	virtual bool isCreature() const override { return true; }
	virtual bool isSpell() const override {return false; }
};

#endif // _CLIENT_CREATURE_CLIENT_HPP
