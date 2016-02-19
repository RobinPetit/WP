#include "server/Card.hpp"

Card::Card(unsigned cost, std::vector<std::vector<unsigned>> effects):
	_cost(cost), _effects(effects)
{

}

std::vector<std::vector<unsigned>> Card::getEffects()
{
	return _effects;
}

unsigned Card::getCost()
{
	return _cost;
}
