#ifndef _SPELL_SERVER_HPP
#define _SPELL_SERVER_HPP

#include "server/Card.hpp"

///Spell card : One of the 2 playables card
class Spell: public Card
{
public:
	/// Constructor
	Spell(const SpellData&);
};

#endif //_SPELL_SERVER_HPP
