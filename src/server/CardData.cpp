#include "server/CardData.hpp"


// CreatureData
CreatureData::CreatureData(cardId id, CostValue cost, const std::vector<EffectParamsCollection>& effects,
                           AttackValue attack, HealthValue health, ShieldValue shield, ShieldType shieldType) :
	CommonCreatureData(id, cost, attack, health, shield, shieldType),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& CreatureData::getEffects() const
{
	return _effects;
}

// SpellCard
SpellData::SpellData(cardId id, CostValue cost, const std::vector<EffectParamsCollection>& effects) :
	CommonSpellData(id, cost),
	_effects(effects)
{
}

const std::vector<EffectParamsCollection>& SpellData::getEffects() const
{
	return _effects;
}
