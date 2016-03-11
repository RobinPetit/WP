#ifndef _CLIENT_SPELL_CLIENT_HPP
#define _CLIENT_SPELL_CLIENT_HPP

#include "client/ClientCard.hpp"

///Spell card : One of the 2 playables card
class ClientSpell: public ClientCard
{
public:
	/// Constructor
	ClientSpell(cardId cardIdentifier, const std::string& name, int cost, const std::string& description);

	virtual inline bool isCreature() override;
	virtual inline bool isSpell() override;
};

#endif // _CLIENT_SPELL_CLIENT_HPP
