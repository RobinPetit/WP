#include "server/Card.hpp"

Card::Card(cardId cardIdentifier, int cost, const std::vector<EffectParamsCollection>& effects):
	_cost(cost),
	_cardId(cardIdentifier),
	_effects(effects)
{

}

const std::vector<EffectParamsCollection>& Card::getEffects() const
{
	return _effects;
}

int Card::getEnergyCost() const
{
	return _cost;
}

cardId Card::getId() const
{
	return _cardId;
}
