#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Card.hpp"


///Monster card : One of the 2 playables card
class Monster : public Card
{
	unsigned int _attack;
	unsigned int _health;

public:

	///Constructors
    Monster(unsigned int cost = 0 , std::string name = "No name", vector<int> effect = {0},
             unsigned int attack = 1, unsigned int health = 1): Card(cost, name, effect),
              _attack(attack), _health(health){};

    ///Getters
    inline unsigned int getHealth(){return _health;}
    inline unsigned int getAttack(){return _attack;}

    ///Setters
	inline void setHealth(unsigned int LP) {_health = LP;}
	inline void setAttack(unsigned int attack) {_attack = attack;}

	///Methods
	inline bool hasEffect() {return (this->getEffect()!= 0);}
};


#endif // MONSTER_HPP
