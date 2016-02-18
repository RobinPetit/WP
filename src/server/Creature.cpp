#include "server/Creature.hpp"

Creature::Creature(	unsigned cost, std::string name,
					std::vector<std::vector<unsigned>> instantEffects,
					std::vector<std::vector<unsigned>> turnEffects,
					std::vector<std::vector<unsigned>> endingEffects,
					unsigned attack, unsigned health):
	Card(cost, name, instantEffects), _turnByTurnEffects(turnEffects), _endingEffects(endingEffects), _attack(attack), _health(health)
{

}

/*--------------------------- EFFECTS */
void Creature::resetAtttack(std::vector<unsigned> args)
{
	 _attack = _attackInit;
}

void Creature::resetHealth(std::vector<unsigned> args)
{
	 _health = _healthInit;
}

void Creature::addHealth(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_health += points;
}

void Creature::subHealth(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
    if(_health > points) _health -= points;
    else
	{
		_health = 0;
		//TODO: creature_dead
	}
}

void Creature::addAttack(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
	_attack += points;
}

void Creature::subAttack(std::vector<unsigned> args)
{
	unsigned points = args.at(0);
    if(_attack > points) _attack -= points;
    else _attack = 0;
}
