#include "server/Card.hpp"

Card::Card(int cost, std::vector<EffectParamsCollection> effects):
	_cost(cost),
	_effects(effects)
{

}

std::vector<EffectParamsCollection> Card::getEffects()
{
	return _effects;
}

int Card::getCost()
{
	return _cost;
}

int Card::getID()
{
	return _cardID;
}
