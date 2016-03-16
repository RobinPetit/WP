#include "server/ServerCardData.hpp"

// ServerCreatureData
ServerCreatureData::ServerCreatureData(cardId id, CostValue cost, const std::vector<EffectParamsCollection>& effects,
                           AttackValue attack, HealthValue health, ShieldValue shield, ShieldType shieldType) :
	CommonCreatureData(id, cost, attack, health, shield, shieldType),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& ServerCreatureData::getEffects() const
{
	return _effects;
}

// SpellCard
ServerSpellData::ServerSpellData(cardId id, CostValue cost, const std::vector<EffectParamsCollection>& effects) :
	CommonSpellData(id, cost),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& ServerSpellData::getEffects() const
{
	return _effects;
}
