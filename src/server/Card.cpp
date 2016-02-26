#include "server/Card.hpp"

Card::Card(int cardID, int cost, std::vector<EffectParamsCollection> effects):
	_cardID(cardID),
	_cost(cost),
	_effects(effects)
{

}

std::vector<EffectParamsCollection> Card::getEffects()
{
	return _effects;
}

int Card::getEnergyCost()
{
	return _cost;
}

int Card::getID()
{
	return _cardID;
}
