#ifndef _CLIENT_SPELL_CLIENT_HPP
#define _CLIENT_SPELL_CLIENT_HPP

#include "common/Card.hpp"
#include "client/ClientCardData.hpp"

///Spell card : One of the 2 playables card
class ClientSpell: public Card
{
private:
	/// used to get good prototype type
	inline const ClientSpellData& prototype() const;

public:
	/// Constructor
	ClientSpell(const ClientSpellData&);

	// GETTERS
	const std::string& getName();
	const std::string& getDescription();
};

#endif // _CLIENT_SPELL_CLIENT_HPP
