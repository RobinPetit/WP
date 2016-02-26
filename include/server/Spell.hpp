#ifndef _SPELL_SERVER_HPP
#define _SPELL_SERVER_HPP

#include "Card.hpp"


///Spell card : One of the 2 playables card
class Spell: public Card
{
public:
	/// Constructor
	Spell(cardId cardIdentifier, int cost, std::vector<EffectParamsCollection> effects);

	virtual inline bool isCreature() override;
	virtual inline bool isSpell() override;
};

#endif //_SPELL_SERVER_HPP
