#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

#include "Card.hpp"


///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	unsigned int _attack, _attackInit;
	unsigned int _health, _healthInit;
    std::vector<std::vector<unsigned>> _turnByTurnEffects;
    std::vector<std::vector<unsigned>> _endingEffects;

public:

	/// Constructors
    Creature(unsigned int cost = 0 , std::string name = "No name", std::vector<std::vector<unsigned>> instantEffects = {{}},
    		std::vector<std::vector<unsigned>> turnEffects = {{}}, std::vector<std::vector<unsigned>> endingEffects = {{}},
            	unsigned int attack = 0, unsigned int health = 1): Card(cost, name, instantEffects),
            		 _turnByTurnEffects(turnEffects), _endingEffects(endingEffects), _attack(attack), _health(health){};

    /// Getters
    inline unsigned int getHealth(){return _health;}
    inline unsigned int getAttack(){return _attack;}

    inline std::vector<std::vector<unsigned>> getTurnByTurnEffects() {return _turnByTurnEffects;}
    inline std::vector<std::vector<unsigned>> getEndingEffects()     {return _endingEffects;}

    /// Effects
	void resetHealth(std::vector<unsigned> args);
	void resetAttack(std::vector<unsigned> args);
    void addHealth(std::vector<unsigned> args);
    void subHealth(std::vector<unsigned> args);
    void addAttack(std::vector<unsigned> args);
    void subAttack(std::vector<unsigned> args);

	/// Methods
	virtual inline bool isCreature() override { return true; };
	virtual inline bool isSpell() override { return false; };

};

#endif // CREATURE_SERVER_HPP
