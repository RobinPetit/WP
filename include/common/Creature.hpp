#ifndef CREATURE_COMMON_HPP
#define CREATURE_COMMON_HPP

#include "Card.hpp"


///Creature card : One of the 2 playables card
class Creature : public Card
{
	unsigned int _attack;
	unsigned int _health;

public:

	///Constructors
    Creature(unsigned int cost = 0 , std::string name = "No name", std::vector<int> effect = {0},
             unsigned int attack = 1, unsigned int health = 1): Card(cost, name, effect),
              _attack(attack), _health(health){};

    ///Getters
    inline unsigned int getHealth(){return _health;}
    inline unsigned int getAttack(){return _attack;}

    ///Setters
	inline void setHealth(unsigned int LP) {_health = LP;}
	inline void setAttack(unsigned int attack) {_attack = attack;}
    
    ///Increments-Decrements
    void incrHealth(unsigned int);
    void decrHealth(unsigned int);
    void incrAttack(unsigned int);
    void decrAttack(unsigned int);
    

	///Methods
	inline bool hasEffect() {return (this->getEffect()!= 0);}
};

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

#endif // CREATURE_COMMON_HPP
