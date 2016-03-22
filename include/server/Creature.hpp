#ifndef CREATURE_SERVER_HPP
#define CREATURE_SERVER_HPP

// Standard header
#include <vector>
#include <functional>
// WizardPoker headers
#include "common/Card.hpp"
#include "server/ServerCardData.hpp"
#include "server/Player.hpp"
#include "server/Constraints.hpp"
#include "common/CardData.hpp" // Why?
#include "common/GameData.hpp" // Why?

// Forward declarations
class Player;

///Creature card : One of the 2 playables card
class Creature : public Card
{
private:
	int _attack;
	int _health;
	int _shield;
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

	/// used to get good prototype type
	inline const ServerCreatureData& prototype() const;

public:
	/// Constructors
	Creature(const ServerCreatureData&, Player& owner);

	/// Player interface
	void moveToBoard();
	void removeFromBoard();
	bool isOnBoard() const;

	void enterTurn();
	void leaveTurn();

	void makeAttack(Creature& victim);
	void receiveAttack(Creature& attacker, int attack, int forced, int loopCount = 0);

	/// Effects interface
	void applyEffectToSelf(EffectArgs effect);

	const std::vector<EffectParamsCollection>& getEffects() const;
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
