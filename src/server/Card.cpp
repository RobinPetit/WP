#include "server/Card.hpp"

Card::Card(cardId cardIdentifier, int cost, const std::vector<EffectParamsCollection>& effects):
	_cost(cost),
	_cardId(cardIdentifier),
	_effects(effects)
{

}

const std::vector<EffectParamsCollection>& Card::getEffects()
{
	return _effects;
}

int Card::getEnergyCost()
{
	return _cost;
}

cardId Card::getId()
{
	return _cardId;
}
