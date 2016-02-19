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

	//Constraints
	ConstraintList _constraints = ConstraintList(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

public:

	/// Constructors
	Creature(unsigned cost=0, unsigned attack=0, unsigned health=1, unsigned shield=0, unsigned shieldType=0,
			std::vector<std::vector<unsigned>> effects = {{}});

	/// Getters
	inline unsigned int getHealth();
	inline unsigned int getAttack();

	/// Effects
	void setConstraint(const std::vector<unsigned>& args);
	void resetAttack(const std::vector<unsigned>& args);
	void resetHealth(const std::vector<unsigned>& args);
	void resetShield(const std::vector<unsigned>& args);
	void addAttack(const std::vector<unsigned>& args);
	void addHealth(const std::vector<unsigned>& args);
	void addShield(const std::vector<unsigned>& args);
	void subAttack(const std::vector<unsigned>& args);
	void subHealth(const std::vector<unsigned>& args);
	void subShield(const std::vector<unsigned>& args);
	void forcedSubHealth(const std::vector<unsigned>& args);

	void (Creature::*effectMethods[C_EFFECTS_COUNT])(const std::vector<unsigned>&) =
	{
		&Creature::setConstraint,
		&Creature::resetAttack,
		&Creature::resetHealth,
		&Creature::resetShield,
		&Creature::addAttack,
		&Creature::addHealth,
		&Creature::addShield,
		&Creature::subAttack,
		&Creature::subHealth,
		&Creature::subShield,
		&Creature::forcedSubHealth
	};


	/// Methods
	virtual inline bool isCreature() override;
	virtual inline bool isSpell() override;

};

#endif // CREATURE_SERVER_HPP
