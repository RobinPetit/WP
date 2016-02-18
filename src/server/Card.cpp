#include "server/Card.hpp"

Card::Card(unsigned int cost, std::vector<std::vector<unsigned>> effects):
	_cost(cost), _instantEffects(effects)
{

}

std::vector<std::vector<unsigned>> Card::getInstantEffects()
{
	return _instantEffects;
}
