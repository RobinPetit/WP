#ifndef CARD_GUI_CLIENT_HPP
#define CARD_GUI_CLIENT_HPP

// std-C++ headers
#include <string>
// External headers
#include <SFML/Graphics.hpp>

class CardGui : public sf::Drawable
{
public:
	/// Constructor.
	CardGui(const std::string& name, const std::string& description, int cost);

	/// Destructor.
	virtual ~CardGui() = default;

	/// Setw the cost of the card.
	void setCost(int cost);

	/// Setw which side of the card is drawn when the card is displayed.
	/// \param true to show the front (with the image, title, ...), false to
	/// show the back.
	void setShownSide(bool showFront);

	/// Positioning interface.
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;

	/// Draws the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	/// The global size of the card, in pixels.
	static const sf::Vector2f SIZE;

	/// The main shape that contains the texture.
	sf::RectangleShape _picture;

	/// The texture displayed in front of the card.
	sf::Texture _pictureTexture;

	/// Ease of use method. It configures the card with the right font, position
	/// , text size and color.
	void setupText(sf::Text& text, const std::string& string, const sf::Vector2f& position) const;

private:
	/// Description graphical text.
	sf::Text _descriptionText;

	/// Name graphical text.
	sf::Text _nameText;

	/// Cost graphical text.
	sf::Text _costText;

	/// Text font.
	sf::Font _font;

	/// Position of the name relatively to the card.
	static const sf::Vector2f NAME_POSITION;

	/// Position of the cost relatively to the card.
	static const sf::Vector2f COST_POSITION;

	/// Position of the description relatively to the card.
	static const sf::Vector2f DESCRIPTION_POSITION;

	/// Character size.
	static constexpr std::size_t CHAR_SIZE = 20;

	/// Current position of the card.
	sf::Vector2f _position;

	/// True when showing the front (with the image, title, ...), false when
	/// showing the back.
	bool _showFront;

	/// Shape that will contains the image of the back side.
	sf::RectangleShape _backView;

	/// Texture of the back side.
	sf::Texture _backTexture;
};



#endif  // CARD_GUI_CLIENT_HPP
