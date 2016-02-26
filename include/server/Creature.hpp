#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

// WizardPoker headers
#include "server/Card.hpp"
#include "server/Player.hpp"
#include "server/Constraints.hpp"
#include "common/CardData.hpp"

///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	int _attack, _attackInit;
	int _health, _healthInit;
	int _shield, _shieldInit;
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

public:
	/// Constructors
	Creature(cardId cardIdentifier, int cost, int attack, int health, int shield, int shieldType,
			std::vector<EffectParamsCollection> effects);

	/// Player interface
	virtual bool isCreature() override;
	virtual bool isSpell() override;
	void movedToBoard();
	void removedFromBoard();
	bool isOnBoard() const;

	void enterTurn();
	void leaveTurn();

	void makeAttack(Creature& victim);
	void receiveAttack(Creature& attacker, int attack, int forced, int loopCount=0);

	/// Effects interface
	void applyEffect(EffectParamsCollection effectArgs);

	int getAttack();
	int getPersonalConstraint(int constraintID) const;
	int getConstraint(int constraintID) const;
};

#endif // CREATURE_SERVER_HPP
