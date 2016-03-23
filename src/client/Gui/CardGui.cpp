// std-C++ headers
#include <iostream>
#include <string>
// WizardPoker headers
#include "client/Gui/CardGui.hpp"
#include <SFML/Graphics.hpp>

const sf::Vector2f CardGui::SIZE{360.f, 240.f};
const sf::Vector2f CardGui::NAME_POSITION{24.f, 36.f};
const sf::Vector2f CardGui::COST_POSITION{300.f, 36.f};
const sf::Vector2f CardGui::DESCRIPTION_POSITION{10.f, 28.f};

CardGui::CardGui(const std::string& name, const std::string& description, int cost):
	_position{0.f, 0.f},
	_showFront{true}
{
	_backTexture.loadFromFile("back.png");
	_backTexture.setSmooth(true); // enable smooth borders
	_backView.setSize(SIZE);
	_backView.setTexture(&_backTexture);

	_font.loadFromFile("TrueLies.ttf"); // loading chosen font

	// complete cost, name and effects
	_nameText.setString(name);
	_nameText.setFillColor(sf::Color::Black);
	_nameText.setFont(_font);
	_descriptionText.setCharacterSize(CHAR_SIZE);
	_nameText.setPosition(NAME_POSITION + _position);

	_costText.setString(std::to_string(cost));
	_costText.setFillColor(sf::Color::Red);
	_costText.setFont(_font);
	_costText.setCharacterSize(CHAR_SIZE);
	_costText.setPosition(COST_POSITION + _position);

	_descriptionText.setString(description);
	_descriptionText.setFillColor(sf::Color::Black);
	_descriptionText.setFont(_font);
	_descriptionText.setCharacterSize(CHAR_SIZE);
	_descriptionText.setPosition(DESCRIPTION_POSITION + _position);
}

void CardGui::setCost(int cost)
{
	_costText.setString(std::to_string(cost));
}

void CardGui::setShownSide(bool showFront)
{
	_showFront = showFront;
}

void CardGui::setPosition(float x, float y)
{
	setPosition({x, y});
}

void CardGui::setPosition(const sf::Vector2f& position)
{
	_position = position;
}

const sf::Vector2f& CardGui::getPosition() const
{
	return _position;
}

void CardGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform.translate(_position);
	if(_showFront)
		target.draw(_backView, states);
	else
		target.draw(_picture, states);
	target.draw(_descriptionText, states);
	target.draw(_nameText, states);
	target.draw(_costText, states);
}
