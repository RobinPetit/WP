#ifndef _CLIENT_SPELL_CLIENT_HPP
#define _CLIENT_SPELL_CLIENT_HPP

#include "common/Card.hpp"
#include "client/CardData.hpp"

///Spell card : One of the 2 playables card
class ClientSpell: public Card
{
private:
	/// used to get good prototype type
	inline const SpellData& prototype() const;

public:
	/// Constructor
	ClientSpell(const SpellData&);

	// GETTERS
	const std::string& getName();
	const std::string& getDescription();
};

#endif // _CLIENT_SPELL_CLIENT_HPP
