#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

// WizardPoker headers
#include "common/Card.hpp"
#include "server/ServerCardData.hpp"
#include "server/Player.hpp"
#include "server/Constraints.hpp"

///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	int _attack;
	int _health;
	int _shield;
	int _shieldType;

	Player* _owner;
	bool _isOnBoard;

	//Constraints
	Constraints _constraints = Constraints(C_CONSTRAINT_DEFAULTS, C_CONSTRAINTS_COUNT);

	//Effects
	static std::function<void(Creature&, const EffectParamsCollection&)> _effectMethods[P_EFFECTS_COUNT];

	void setConstraint(const EffectParamsCollection& args);
	void resetAttack(const EffectParamsCollection& args);
	void resetHealth(const EffectParamsCollection& args);
	void resetShield(const EffectParamsCollection& args);
	void changeAttack(const EffectParamsCollection& args);
	void changeHealth(const EffectParamsCollection& args);
	void changeShield(const EffectParamsCollection& args);
	void forcedChangeHealth(const EffectParamsCollection& args);

	/// used to get good prototype type
	inline const ServerCreatureData& prototype() const;

public:
	/// Constructors
	Creature(const ServerCreatureData&);

	/// Player interface
	void movedToBoard();
	void removedFromBoard();
	bool isOnBoard() const;

	void enterTurn();
	void leaveTurn();

	void makeAttack(Creature& victim);
	void receiveAttack(Creature& attacker, int attack, int forced, int loopCount = 0);

	/// Effects interface
	void applyEffectToSelf(EffectParamsCollection effectArgs);

	std::vector<EffectParamsCollection> getEffects();
	int getAttack();
	int getHealth();
	int getShield();
	int getShieldType();
	int getPersonalConstraint(int constraintID) const;
	int getConstraint(int constraintID) const;
};

#endif // CREATURE_SERVER_HPP
