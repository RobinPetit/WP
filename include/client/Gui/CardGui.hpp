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
	void setCost(int cost);

	/// Set which side of the card is drawn when the card is displayed.
	/// \param true to show the front (with the image, title, ...), false to
	/// show the back.
	void setShownSide(bool showFront);

protected:
	// name,cost, and effects
	sf::Text _descriptionText;
	sf::Text _nameText;
	sf::Text _costText;

	//loading characters font
	sf::Font _font;

	// Arbitrary shape
	static const sf::Vector2f SIZE;
	static const sf::Vector2f NAME_POSITION;
	static const sf::Vector2f COST_POSITION;
	static const sf::Vector2f DESCRIPTION_POSITION;
	static constexpr std::size_t CHAR_SIZE = 20;

	sf::Vector2f _position;
	bool _showFront;

	sf::RectangleShape _backView;
	sf::Texture _backTexture;

	sf::RectangleShape _picture;
	sf::Texture _pictureTexture;
};



#endif  // CARD_GUI_CLIENT_HPP
