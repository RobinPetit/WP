#include "server/Card.hpp"

Card::Card(unsigned int cost = 0, std::string name = "No name" , std::vector<std::vector<unsigned>> effect = {{}}):
	_cost(cost), _name(name), _instantEffects(effect)
{

}

std::vector<std::vector<unsigned>> Card::getInstantEffects()
{
	return _instantEffects;
}
