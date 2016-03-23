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
	} // Standard protocol SFML 2.0 error (CF documentation)
	_pictureTexture.setSmooth(true);
	_picture.setTexture(&_pictureTexture);

	_attack = attack;
	_health = health;
	_shield = shield;
	_shieldType = shieldType;
}
