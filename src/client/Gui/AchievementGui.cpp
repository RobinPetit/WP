// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Gui/AchievementGui.hpp"

const sf::Vector2f AchievementGui::SIZE{350.f, 72.f};
const sf::Vector2f AchievementGui::IMAGE_SIZE{57.2f, 72.f};
const sf::Vector2f AchievementGui::IMAGE_POSITION{0.f, 0.f};
const sf::Vector2f AchievementGui::NAME_POSITION{60.f, 0.f};
const sf::Vector2f AchievementGui::DESCRIPTION_POSITION{60.f, 25.f};
constexpr char AchievementGui::IMAGE_LOCKED_PATH[];
constexpr char AchievementGui::IMAGE_UNLOCKED_PATH[];
constexpr char AchievementGui::FONT_PATH[];

AchievementGui::AchievementGui(const std::string& name, const std::string& description, bool isUnlocked):
	_isUnlocked{isUnlocked}
{
	if (!_isUnlocked and !_imageTexture.loadFromFile(IMAGE_LOCKED_PATH))
	{
		std::cerr << "Unable to load " << IMAGE_LOCKED_PATH << "\n";
		return;
	}

	if (_isUnlocked and !_imageTexture.loadFromFile(IMAGE_UNLOCKED_PATH))
	{
		std::cerr << "Unable to load " << IMAGE_UNLOCKED_PATH << "\n";
		return;
	}

	if(!_font.loadFromFile(FONT_PATH))
	{
		std::cerr << "Unable to load " << FONT_PATH << "\n";
		return;
	}

	// background
	_bgRectangle.setSize(SIZE);
	_bgRectangle.setFillColor({179,202,179});

	// image
	_imageTexture.setSmooth(true);
	_imageSprite.setTexture(_imageTexture);
	_imageSprite.setScale(0.1f, 0.1f);

	// text
	setupText(_nameText, name, NAME_POSITION);
	_nameText.setStyle(sf::Text::Bold);
	_nameText.setCharacterSize(NAME_CHAR_SIZE);

	setupText(_descriptionText, description, DESCRIPTION_POSITION);
	_descriptionText.setStyle(sf::Text::Italic);
	_descriptionText.setCharacterSize(DESCRIPTION_CHAR_SIZE);
}

sf::Vector2f AchievementGui::getSize()
{
	return SIZE;
}

void AchievementGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform.combine(getTransform());
	target.draw(_bgRectangle, states);
	target.draw(_descriptionText, states);
	target.draw(_nameText, states);
	target.draw(_imageSprite, states);
}

void AchievementGui::setupText(sf::Text& text, std::string string, const sf::Vector2f& position) const
{
	float lineSize{0.f};
	float spaceSize = _font.getGlyph(static_cast<sf::Uint32>(' '), DESCRIPTION_CHAR_SIZE, false).advance;
	float maxWidth = SIZE.x - NAME_POSITION.x;

	auto prevIt = string.begin();
	for(auto it(string.begin()); it != string.end(); it++)
	{
		if (*it==' ')
		{
			while (*it==' ')
			{
				lineSize += spaceSize;
				it++;
			}
			prevIt = it;
		}

		lineSize += _font.getGlyph(static_cast<sf::Uint32>(*it), DESCRIPTION_CHAR_SIZE, false).advance;

		if(lineSize >= maxWidth)
		{
			lineSize = 0.f;
			it = string.insert(prevIt, '\n');
		}
	}

	text.setString(string);
	text.setColor(sf::Color::Black);
	text.setFont(_font);
	text.setPosition(position);
}
