#ifndef _CLIENT_SPELL_CLIENT_HPP
#define _CLIENT_SPELL_CLIENT_HPP

#include "client/ClientCard.hpp"

///Spell card : One of the 2 playables card
class ClientSpell: public ClientCard
{
public:
	/// Constructor
	ClientSpell(cardId cardIdentifier, const std::string& name, int cost, const std::string& description);

	virtual bool isCreature() const override { return false; };
	virtual bool isSpell() const override { return true; };
};

#endif // _CLIENT_SPELL_CLIENT_HPP
