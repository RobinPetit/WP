#ifndef CARD_GUI_CLIENT_HPP
#define CARD_GUI_CLIENT_HPP

// External headers
#include <SFML/Graphics.hpp>

class CardGui
{
public:
	/// Constructor.
	CardGui(const std::string& name, const std::string& description, int cost);

	/// Destructor.
	virtual ~CardGui() = default;

	/// Setters
	void setCost(int cost) {_costText.setString(std::to_string(cost));}

protected:
	// name,cost, and effects
	sf::Text _descriptionText;
	sf::Text _nameText;
	sf::Text _costText;

	//loading characters font
	sf::Font _font;
	// Arbitrary shape
	static constexpr float LENGTH = 360.f;
	static constexpr float WIDTH  = 240.f;

	static constexpr float NAME_POSITION_X = 24.f;
	static constexpr float NAME_POSITION_Y = 36.f;

	static constexpr float EFFECTS_POSITION_X = 10.f;
	static constexpr float EFFECTS_POSITION_Y = 28.f;

	static constexpr std::size_t CHAR_SIZE = 20;

	/* Back view texture and sprite only, because the front
	one depends on if it's a spell or creature card*/
	sf::RectangleShape _backView;
	sf::Texture _backTexture;

	sf::RectangleShape _picture;
	sf::Texture _pictureTexture;
};



#endif  // CARD_GUI_CLIENT_HPP
