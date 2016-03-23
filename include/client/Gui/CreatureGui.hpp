#ifndef CREATURE_GUI_CLIENT_HPP
#define CREATURE_GUI_CLIENT_HPP

#include "client/Gui/CardGUI.hpp"

class CreatureGui: public CardGui
{
private:
    int _attack;
	int _health;
	int _shield;
	int _shieldType;

public:
	/// Constructor
	CreatureGui(std::string cardIdentifier, int cost, int attack, int health, int shield, int shieldType,
			std::vector<EffectParamsCollection> effects) // Taking only the required informations for the GUI


    /// Setters
    void setAttack(int value) {_attack = value;}
    void setHealth(int value) {_health = value;}
    void setShield(int value) {_shield = value;}

};

#endif  // CREATURE_GUI_CLIENT_HPP
