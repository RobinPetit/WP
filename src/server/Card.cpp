#include "server/Card.hpp"

Card::Card(unsigned cost, std::vector<EffectParamsCollection> effects):
	_cost(cost), _effects(effects)
{

}

std::vector<EffectParamsCollection> Card::getEffects()
{
	return _effects;
}

unsigned Card::getCost()
{
	return _cost;
}
