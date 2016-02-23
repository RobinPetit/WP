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
	int _attack, _attackInit;
	int _health, _healthInit;
	int _shield, _shieldInit;
	int _shieldType;

	Player* _owner;

	//Constraints
	Constraints _constraints = Constraints(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

public:
	typedef void (Creature::*CreatureEffectMethod)(const EffectParamsCollection&);
	/// Constructors
	Creature(int cost=0, int attack=0, int health=1, int shield=0, int shieldType=0,
			std::vector<EffectParamsCollection> effects = {{}});

	/// Getters
	int getAttack();
	int getHealth();
	virtual bool isCreature() override;
	virtual bool isSpell() override;
	int getConstraint(int constraintID);

	/// Player interface
	void enterTurn();
	void leaveTurn();

	/// Effects
	void setConstraint(const EffectParamsCollection& args);
	void resetAttack(const EffectParamsCollection& args);
	void resetHealth(const EffectParamsCollection& args);
	void resetShield(const EffectParamsCollection& args);
	void changeAttack(const EffectParamsCollection& args);
	void changeHealth(const EffectParamsCollection& args);
	void changeShield(const EffectParamsCollection& args);
	void forcedChangeHealth(const EffectParamsCollection& args);

	static std::function<void(Creature&, const EffectParamsCollection&)> effectMethods[P_EFFECTS_COUNT];
};

#endif // CREATURE_SERVER_HPP
