#ifndef _SPELL_SERVER_HPP
#define _SPELL_SERVER_HPP

#include "Card.hpp"


///Spell card : One of the 2 playables card
class Spell: public Card
{
	public:
		virtual inline bool isCreature() override { return true; };
		virtual inline bool isSpell() override { return false; };
};

#endif //_SPELL_SERVER_HPP
