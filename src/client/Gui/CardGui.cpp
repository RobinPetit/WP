// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Gui/CardGui.hpp"

const sf::Vector2f CardGui::SIZE{260.f, 360.f};
const sf::Vector2f CardGui::NAME_POSITION{20.f, 17.f};
const sf::Vector2f CardGui::COST_POSITION{220.f, 17.f};
constexpr char CardGui::BACK_IMAGE_PATH[];
constexpr char CardGui::FONT_PATH[];

CardGui::CardGui(const std::string& name, std::string description, int cost):
	_showFront{true}
{
	if(!_backTexture.loadFromFile(BACK_IMAGE_PATH))
	{
		std::cerr << "Unable to load " << BACK_IMAGE_PATH << "\n";
		return;
	}
	_backTexture.setSmooth(true); // enable smooth borders
	_backView.setSize(SIZE);
	_backView.setTexture(&_backTexture);

	if(!_font.loadFromFile(FONT_PATH))
	{
		std::cerr << "Unable to load " << FONT_PATH << "\n";
		return;
	}

	// complete cost, name and effects
	setupText(_nameText, name, NAME_POSITION);
	_nameText.setStyle(sf::Text::Bold);

	setupText(_costText, std::to_string(cost), COST_POSITION);
	_costText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	_costText.setColor(sf::Color(0, 0, 100));

	addNewlinesInDescription(description);
	setupText(_descriptionText, description, {0, 0});
	_descriptionText.setStyle(sf::Text::Italic);
	_descriptionText.setCharacterSize(DESCRIPTION_CHAR_SIZE);
}

void CardGui::setCost(int cost)
{
	_costText.setString(std::to_string(cost));
}

void CardGui::setShownSide(bool showFront)
{
	_showFront = showFront;
}

sf::Vector2f CardGui::getSize()
{
	return SIZE;
}

void CardGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform.combine(getTransform());
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
	text.setColor(sf::Color::Black);
	text.setFont(_font);
	text.setCharacterSize(CHAR_SIZE);
	text.setPosition(position);
}

void CardGui::moveDescription(const sf::Vector2f& descriptionPosition)
{
	_descriptionText.setPosition(descriptionPosition);
}

void CardGui::addNewlinesInDescription(std::string& description)
{
	float lineSize{0.f};
	const float spaceSize{getDescriptionCharacterWidth(' ')};

	auto prevIt = description.begin();
	for(auto it(description.begin()); it != description.end(); ++it)
	{
		if(*it == ' ')
		{
			while(*it == ' ')
			{
				lineSize += spaceSize;
				it++;
			}
			prevIt = it;
		}

		lineSize += getDescriptionCharacterWidth(*it);

		if(lineSize >= DESCRIPTION_WIDTH)
		{
			lineSize = 0.f;
			it = description.insert(prevIt, '\n');
		}
	}
}

inline float CardGui::getDescriptionCharacterWidth(char character) const
{
	 return _font.getGlyph(static_cast<sf::Uint32>(character), DESCRIPTION_CHAR_SIZE, false).advance;
}
