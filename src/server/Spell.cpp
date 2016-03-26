#include "server/Spell.hpp"

Spell::Spell(const ServerSpellData& cardData):
	Card(cardData)
{
}

std::vector<EffectParamsCollection> Spell::getEffects()
{
	return prototype().getEffects();
}

/*--------------------------- PRIVATE METHOD */
inline const ServerSpellData& Spell::prototype() const
{
	return static_cast<const ServerSpellData&>(_prototype);
}
