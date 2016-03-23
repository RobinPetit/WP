#ifndef _SPELL_SERVER_HPP
#define _SPELL_SERVER_HPP

#include "common/Card.hpp"
#include "server/ServerCardData.hpp"

///Spell card : One of the 2 playables card
class Spell: public Card
{
private:
	/// used to get good prototype type
	inline const ServerSpellData& prototype() const;

public:
	/// Constructor
	Spell(const ServerSpellData&);

	/// Effects interface
	std::vector<EffectParamsCollection> getEffects();
};

#endif //_SPELL_SERVER_HPP
