#include "server/Creature.hpp"

void Creature::incrHealth(unsigned int LP) {_health+=LP;}

void Creature::decrHealth(unsigned int LP)
{
    if(_health > LP){
        _health -= LP;}
    else
        _health = 0;

}

void Creature::incrAttack(unsigned int attack) {_attack+=attack;}

void Creature::decrAttack(unsigned int attack)
{
    if(_attack > attack){
        _attack -= attack;}
    else
        _attack = 0;
}
