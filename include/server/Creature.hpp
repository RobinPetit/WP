#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

#include "server/Card.hpp"
#include "server/Constraints.hpp"


///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	unsigned _attack, _attackInit;
	unsigned _health, _healthInit;
	unsigned _shield, _shieldInit;
	unsigned _shieldType;
    std::vector<std::vector<unsigned>> _turnEffects;
    std::vector<std::vector<unsigned>> _endingEffects;

    //Constraints
    ConstraintList _constraints = ConstraintList(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

public:

	/// Constructors
    Creature(unsigned cost=0, unsigned attack=0, unsigned health=1, unsigned shield=0, unsigned shieldType=0,
			std::vector<std::vector<unsigned>> instantEffects = {{}},
    		std::vector<std::vector<unsigned>> turnEffects = {{}},
    		std::vector<std::vector<unsigned>> endingEffects = {{}});

    /// Getters
    inline unsigned int getHealth(){return _health;}
    inline unsigned int getAttack(){return _attack;}

    inline std::vector<std::vector<unsigned>> getTurnEffects() {return _turnEffects;}
    inline std::vector<std::vector<unsigned>> getEndingEffects() {return _endingEffects;}

    /// Effects
    void setConstraint(std::vector<unsigned> args);

    void resetAttack(std::vector<unsigned> args);
	void resetHealth(std::vector<unsigned> args);
	void resetShield(std::vector<unsigned> args);

	void addAttack(std::vector<unsigned> args);
    void addHealth(std::vector<unsigned> args);
    void addShield(std::vector<unsigned> args);

	void subAttack(std::vector<unsigned> args);
    void subHealth(std::vector<unsigned> args);
    void subShield(std::vector<unsigned> args);

	void forcedSubHealth(std::vector<unsigned> args);


	/// Methods
	virtual inline bool isCreature() override { return true; };
	virtual inline bool isSpell() override { return false; };

};

#endif // CREATURE_SERVER_HPP
