#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

// Standard header
#include <vector>
#include <functional>
// WizardPoker headers
#include "server/Card.hpp"
#include "server/Constraints.hpp"
#include "common/CardData.hpp"
#include "common/GameData.hpp"

// Forward declarations
class Player;

///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	int _attack, _attackInit;
	int _health, _healthInit;
	int _shield, _shieldInit;
	int _shieldType;

	Player& _owner;
	bool _isOnBoard;

	//Constraints
	Constraints _constraints = Constraints(creatureDefaultConstraints);

	//Effects
	static std::array<std::function<void(Creature&, EffectArgs)>, P_EFFECTS_COUNT> _effectMethods;

	void setConstraint(EffectArgs effect);
	void resetAttack(EffectArgs effect);
	void resetHealth(EffectArgs effect);
	void resetShield(EffectArgs effect);
	void changeAttack(EffectArgs effect);
	void changeHealth(EffectArgs effect);
	void changeShield(EffectArgs effect);
	void forcedChangeHealth(EffectArgs effect);

public:
	/// Constructors
	Creature(cardId cardIdentifier, Player& owner, int cost, int attack, int health, int shield, int shieldType,
			std::vector<EffectParamsCollection> effects);

	/// Player interface
	virtual bool isCreature() override;
	virtual bool isSpell() override;
	void moveToBoard();
	void removeFromBoard();
	bool isOnBoard() const;

	void enterTurn();
	void leaveTurn();

	void makeAttack(Creature& victim);
	void receiveAttack(Creature& attacker, int attack, int forced, int loopCount=0);

	/// Effects interface
	void applyEffectToSelf(EffectArgs effect);

	int getAttack() const;
	int getHealth() const;
	int getShield() const;
	int getShieldType() const;
	int getPersonalConstraint(int constraintId) const;
	int getConstraint(int constraintId) const;
	bool getConstraintBool(int constraintId) const;

	explicit operator BoardCreatureData() const;
};

#endif // CREATURE_SERVER_HPP
