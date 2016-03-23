#include "client/Gui/CardGui.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

CardGUI::CardGUI(std::string effects, std::string name, std::string cost)
{
    _backTexture.loadFromFile("back.png");
    _backTexture.setSmooth(true); // enable smooth borders
    _backView.setSize(sf::Vector2f(WIDTH, LENGTH));
    _backView.setTexture(&_backTexture);

    _font.loadFromFile("TrueLies.ttf"); // loading chosen font

    //complete cost, name and effects
    _name.setString(name + "\t" + cost);_name.setColor(sf::Color::Red);
    _name.setFont(_font);
    _name.setPosition(sf::Vector2f(WIDTH/NAME_POSITION_X, LENGTH/NAME_POSITION_Y));

    _effects.setString(effects);_effects.setColor(sf::Color::Black);
    _effects.setFont(_font);_effects.setCharacterSize(CHAR_SIZE);
    _effects.setPosition(sf::Vector2f(WIDTH/EFFECTS_POSITION_X, (LENGTH/NAME_POSITION_Y)*EFFECTS_POSITION_Y));

    //~ TODO ADDING PICTURE TO CONSTRUCTOR

}
CardGUI::void drawBack()
{
    /*Just drawing the back of the cards without any other element */
    _backView.draw()
}



