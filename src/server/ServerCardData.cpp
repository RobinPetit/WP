#include "server/ServerCardData.hpp"

// ServerCreatureData
ServerCreatureData::ServerCreatureData(cardId id, int cost, const std::vector<EffectParamsCollection>& effects,
                           int attack, int health, int shield, int shieldType) :
	CommonCreatureData(id, cost, attack, health, shield, shieldType),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& ServerCreatureData::getEffects() const
{
	return _effects;
}

// SpellCard
ServerSpellData::ServerSpellData(cardId id, int cost, const std::vector<EffectParamsCollection>& effects) :
	CommonSpellData(id, cost),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& ServerSpellData::getEffects() const
{
	return _effects;
}
