// std-C++ headers
#include <iostream>
#include <string>
// WizardPoker headers
#include "client/Gui/CardGui.hpp"
#include <SFML/Graphics.hpp>

CardGui::CardGui(const std::string& name, const std::string& description, int cost)
{
	_backTexture.loadFromFile("back.png");
	_backTexture.setSmooth(true); // enable smooth borders
	_backView.setSize(sf::Vector2f(WIDTH, LENGTH));
	_backView.setTexture(&_backTexture);

	_font.loadFromFile("TrueLies.ttf"); // loading chosen font

	// complete cost, name and effects
	_nameText.setString(name + "\t" + std::to_string(cost));
	_nameText.setColor(sf::Color::Red);
	_nameText.setFont(_font);
	_nameText.setPosition(sf::Vector2f(WIDTH/NAME_POSITION_X, LENGTH/NAME_POSITION_Y));

	_descriptionText.setString(description);
	_descriptionText.setColor(sf::Color::Black);
	_descriptionText.setFont(_font);
	_descriptionText.setCharacterSize(CHAR_SIZE);
	_descriptionText.setPosition(sf::Vector2f(WIDTH/EFFECTS_POSITION_X, (LENGTH/NAME_POSITION_Y)*EFFECTS_POSITION_Y));

	// TODO ADDING PICTURE TO CONSTRUCTOR
}
