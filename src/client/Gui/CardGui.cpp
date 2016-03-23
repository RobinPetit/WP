// std-C++ headers
#include <iostream>
#include <string>
// WizardPoker headers
#include "client/Gui/CardGui.hpp"
#include <SFML/Graphics.hpp>

const sf::Vector2f CardGui::SIZE{240.f, 360.f};
const sf::Vector2f CardGui::NAME_POSITION{12.f, 12.f};
const sf::Vector2f CardGui::COST_POSITION{200.f, 12.f};
const sf::Vector2f CardGui::DESCRIPTION_POSITION{300.f, 12.f};

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
	setupText(_nameText, name, NAME_POSITION);
	setupText(_costText, std::to_string(cost), COST_POSITION);
	_costText.setFillColor(sf::Color::Red);
	setupText(_descriptionText, description, DESCRIPTION_POSITION);
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
	{
		target.draw(_picture, states);
		target.draw(_descriptionText, states);
		target.draw(_nameText, states);
		target.draw(_costText, states);
	}
	else
		target.draw(_backView, states);
}

void CardGui::setupText(sf::Text& text, const std::string& string, const sf::Vector2f& position) const
{
	text.setString(string);
	text.setFillColor(sf::Color::Black);
	text.setFont(_font);
	text.setCharacterSize(CHAR_SIZE);
	text.setPosition(position);
}
