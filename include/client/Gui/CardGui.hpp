#ifndef CARD_GUI_CLIENT_HPP
#define CARD_GUI_CLIENT_HPP

// External headers
#include <SFML/Graphics.hpp>

class CardGui : public sf::Drawable
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

	/// Positioning interface.
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;

	/// Draw the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	/// Description graphical text.
	sf::Text _descriptionText;

	/// Name graphical text.
	sf::Text _nameText;

	/// Cost graphical text.
	sf::Text _costText;

	/// Text font.
	sf::Font _font;

	// Arbitrary shape
	static const sf::Vector2f SIZE;
	static const sf::Vector2f NAME_POSITION;
	static const sf::Vector2f COST_POSITION;
	static const sf::Vector2f DESCRIPTION_POSITION;
	static constexpr std::size_t CHAR_SIZE = 20;

	/// Current position of the card.
	sf::Vector2f _position;

	/// True when showing the front (with the image, title, ...), false when
	/// showing the back.
	bool _showFront;

	sf::RectangleShape _backView;
	sf::Texture _backTexture;

	sf::RectangleShape _picture;
	sf::Texture _pictureTexture;

	/// Ease of use method.
	void setupText(sf::Text& text, const std::string& string, const sf::Vector2f& position) const;
};



#endif  // CARD_GUI_CLIENT_HPP
