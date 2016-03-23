// std-C++ headers
#include <iostream>
#include <string>
// WizardPoker headers
#include "client/Gui/CardGui.hpp"
#include <SFML/Graphics.hpp>

const sf::Vector2f CardGui::SIZE{360.f, 240.f};
const sf::Vector2f CardGui::NAME_POSITION{24.f, 36.f};
const sf::Vector2f CardGui::DESCRIPTION_POSITION{10.f, 28.f};

CardGui::CardGui(const std::string& name, const std::string& description, int cost):
	_position{0.f, 0.f}
{
	_backTexture.loadFromFile("back.png");
	_backTexture.setSmooth(true); // enable smooth borders
	_backView.setSize(SIZE);
	_backView.setTexture(&_backTexture);

	_font.loadFromFile("TrueLies.ttf"); // loading chosen font

	// complete cost, name and effects
	_nameText.setString(name + "\t" + std::to_string(cost));
	_nameText.setColor(sf::Color::Red);
	_nameText.setFont(_font);
	_nameText.setPosition(NAME_POSITION + _position);

	_descriptionText.setString(description);
	_descriptionText.setColor(sf::Color::Black);
	_descriptionText.setFont(_font);
	_descriptionText.setCharacterSize(CHAR_SIZE);
	_descriptionText.setPosition(DESCRIPTION_POSITION + _position);
}
