#include "server/Spell.hpp"

Spell::Spell(const SpellData& cardData):
	Card(cardData)
{
}

std::vector<EffectParamsCollection> Spell::getEffects()
{
	return prototype().getEffects();
}

/*--------------------------- PRIVATE METHOD */
inline const SpellData& Spell::prototype() const
{
	return static_cast<const SpellData&>(_prototype);
}
