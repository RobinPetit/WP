#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

// WizardPoker headers
#include "server/Card.hpp"
#include "server/Player.hpp"
#include "server/Constraints.hpp"
#include "server/CardData.hpp"

///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	unsigned _attack, _attackInit;
	unsigned _health, _healthInit;
	unsigned _shield, _shieldInit;
	unsigned _shieldType;

	Player* _owner;

	//Constraints
	Constraints _constraints = Constraints(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

public:
	typedef void (Creature::*CreatureEffectMethod)(const EffectParamsCollection&);
	/// Constructors
	Creature(unsigned cost=0, unsigned attack=0, unsigned health=1, unsigned shield=0, unsigned shieldType=0,
			std::vector<EffectParamsCollection> effects = {{}});

	/// Getters
	unsigned getAttack();
	unsigned getHealth();
	virtual bool isCreature() override;
	virtual bool isSpell() override;
	unsigned getConstraint(unsigned constraintID);

	/// Player interface
	void enterTurn();
	void leaveTurn();

	/// Effects
	void setConstraint(const EffectParamsCollection& args);
	void resetAttack(const EffectParamsCollection& args);
	void resetHealth(const EffectParamsCollection& args);
	void resetShield(const EffectParamsCollection& args);
	void addAttack(const EffectParamsCollection& args);
	void addHealth(const EffectParamsCollection& args);
	void addShield(const EffectParamsCollection& args);
	void subAttack(const EffectParamsCollection& args);
	void subHealth(const EffectParamsCollection& args);
	void subShield(const EffectParamsCollection& args);
	void forcedSubHealth(const EffectParamsCollection& args);

	static std::function<void(Creature&, const EffectParamsCollection&)> effectMethods[P_EFFECTS_COUNT];
};

#endif // CREATURE_SERVER_HPP
