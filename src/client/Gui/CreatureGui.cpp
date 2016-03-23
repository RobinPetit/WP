// std-C++ headers
#include <iostream>
// WizardPoker header
#include "client/Gui/CreatureGui.hpp"

CreatureGui::CreatureGui(const std::string& name, const std::string& description, int cost, int attack, int health, int shield, int shieldType):
	CardGui(name, description, cost)
{
	if(!_pictureTexture.loadFromFile("frontCreature.png"))
	{
		std::cerr << "Error front loading" ;
		return;
	} // Standard SFML 2.0 error protocol (CF documentation)
	_pictureTexture.setSmooth(true);
	_picture.setTexture(&_pictureTexture);
	_picture.setSize(SIZE);

	_attack = attack;
	_health = health;
	_shield = shield;
	_shieldType = shieldType;
}

void CreatureGui::setAttack(int value)
{
	_attack = value;
}

void CreatureGui::setHealth(int value)
{
	_health = value;
}

void CreatureGui::setShield(int value)
{
	_shield = value;
}
