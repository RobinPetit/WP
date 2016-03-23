#ifndef CREATURE_GUI_CLIENT_HPP
#define CREATURE_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"

class CreatureGui: public CardGui
{
public:
	/// Constructor.
	CreatureGui(const std::string& name, const std::string& description, int cost, int attack, int health, int shield, int shieldType);

	/// Setters
	void setAttack(int value);
	void setHealth(int value);
	void setShield(int value);

private:
	int _attack;
	int _health;
	int _shield;
	int _shieldType;
};

#endif  // CREATURE_GUI_CLIENT_HPP
