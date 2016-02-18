#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

#include "Card.hpp"


///Creature card : One of the 2 playables card
class Creature : public Card
{
	unsigned int _attack, _attackInit;
	unsigned int _health, _healthInit;

public:

	/// Constructors
    Creature(unsigned int cost = 0 , std::string name = "No name", std::vector<std::vector<unsigned>> effect = {},
            unsigned int attack = 1, unsigned int health = 1): Card(cost, name, effect),        //ici
                _attack(attack), _health(health){};

    /// Getters
    inline unsigned int getHealth(){return _health;}
    inline unsigned int getAttack(){return _attack;}

    /// Re-Setters
	inline void resetHealth() {_health = _healthInit;}
	inline void resetAttack() {_attack = _attackInit;}

    /// Increments-Decrements
    void incrHealth(unsigned int);
    void decrHealth(unsigned int);
    void incrAttack(unsigned int);
    void decrAttack(unsigned int);

	/// Methods
	virtual inline bool isCreature() { return true; } override;
	virtual inline bool isSpell() { return false; } override;

};

#endif // CREATURE_SERVER_HPP
