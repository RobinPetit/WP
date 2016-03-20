#include "CreatureGUI.hpp"


CreatureGUI::CreatureGUI(std::string cardIdentifier, int cost, int attack, int health, int shield, int shieldType,
			std::vector<EffectParamsCollection> effects)
{
    if (!_backTexture.loadFromFile("back.png"))
        {
            std::cout << "Error back loading" ;
        }// Standarde protocole SFML 2.0 error ( CF documentations )
    _frontTexture.setSmooth(true); // enable smooth borders

    if (!_frontTexture.loadFromFile("frontCreature.png"))
        {
            std::cout << "Error front loading" ;
        }// Standarde protocole SFML 2.0 error ( CF documentations )
    _frontTexture.setSmooth(true);

    _backView.setTexture(_backTexture);
    _frontView.setTexture(_frontTexture);

    _attack = attack;
	_health = health;
	_shield = shield;
	_shieldType = shieldType;
}
