#ifndef MONSTER_HPP
#define MONSTER_HPP


#include "card.hpp"


///Monster card : One of the 2 playables card
class Monster : public Card
{
	unsigned int _attack;
	unsigned int _health;

public:

	///TODO
	///Constructors
	/* Depend on Card class */

    ///Getters
    inline unsigned int getHealth(){return _health;};
    inline unsigned int getAttack(){return _attack;};

    ///Setters
	inline void setHealth(unsigned int LP) {_health = LP;};	
	inline void setAttack(unsigned int attack) {_attack = attack;};

	///Methods
	inline bool hasEffect() const {return _effect != 0;};



};


#endif // MONSTER_HPP
