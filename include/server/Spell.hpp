#ifndef _SPELL_SERVER_HPP
#define _SPELL_SERVER_HPP

#include "Card.hpp"


///Spell card : One of the 2 playables card
class Spell: public Card
{
	public:
		virtual inline bool isCreature() { return true; } override;
		virtual inline bool isSpell() { return false; } override;
};

#endif //_SPELL_SERVER_HPP
