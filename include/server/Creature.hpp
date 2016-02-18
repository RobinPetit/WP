#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

#include "Card.hpp"


///Creature card : One of the 2 playables card
class Creature : public Card
{
	std::vector<std::vector<unsigned>> _turnByTurnEffects; 
    std::vector<std::vector<unsigned>> _endingEffects; 
	unsigned int _attack, _attackInit;
	unsigned int _health, _healthInit;


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
	virtual bool hasEffects() { return not (_instantEffects.empty() && _turnByTurnEffects.empty() && _endingEffects.empty()) ;} override;

};

#endif // CREATURE_SERVER_HPP
