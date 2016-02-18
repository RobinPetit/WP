#include "server/Card.hpp"

Card::Card(unsigned cost, std::vector<std::vector<unsigned>> instantEffects):
	_cost(cost), _instantEffects(instantEffects)
{

}

std::vector<std::vector<unsigned>> Card::getInstantEffects()
{
	return _instantEffects;
}
