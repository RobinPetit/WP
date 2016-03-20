#include "SpellGUI.hpp"


SpellGUI::SpellGUI(std::string cardIdentifier, int cost, std::vector<EffectParamsCollection> effects)
{
    if (!_backTexture.loadFromFile("back.png"))
        {
            std::cout << "Error back loading" ;
        }// Standarde protocole SFML 2.0 error ( CF documentations )
    _frontTexture.setSmooth(true); // enable smooth borders

    if (!_frontTexture.loadFromFile("frontSpell.png"))
        {
            std::cout << "Error front loading" ;
        }// Standarde protocole SFML 2.0 error ( CF documentations )
    _frontTexture.setSmooth(true);

    _backView.setTexture(_backTexture);
    _frontView.setTexture(_frontTexture);

    _cardID = cardIdentifier;
    _cost = cost;
    _effects = effects;
}

