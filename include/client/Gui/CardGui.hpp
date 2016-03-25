#ifndef CARD_GUI_CLIENT_HPP
#define CARD_GUI_CLIENT_HPP

// std-C++ headers
#include <string>
// External headers
#include <SFML/Graphics.hpp>

/// Graphical representation of a card. It behaves like other SFML graphicals
/// classes, plus some getters/setters for the card interface.
class CardGui : public sf::Drawable, public sf::Transformable
{
public:
	/// Constructor.
	/// \note The description parameter is taken by value on purpose.
	CardGui(const std::string& frontTexturePath, const std::string& name, std::string description, int cost);

	/// Destructor.
	virtual ~CardGui() = default;

	/// Sets the cost of the card.
	void setCost(int cost);

	/// Sets which side of the card is drawn when the card is displayed.
	/// \param true to show the front (with the image, title, ...), false to
	/// show the back.
	void setShownSide(bool showFront);

	/// Returns the size of a card. Since all cards have the same size, this
	/// method is static.
	/// \return the size of a card.
	static sf::Vector2f getSize();

	/// Draws the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	/// The global size of the card, in pixels.
	static const sf::Vector2f SIZE;

	/// The main shape that contains the texture.
	sf::Sprite _picture;

	/// The texture displayed in front of the card.
	sf::Texture _pictureTexture;

	/// Ease of use method. It configures the card with the right font, position
	/// , text size and color.
	void setupText(sf::Text& text, const std::string& string, const sf::Vector2f& position) const;

	/// Sets the position of the description. This is needed because different
	/// kind of cards may place the description in different places, so child
	/// classes have to explicitely say where the description has to be.
	/// \param descriptionPosition The new position of the description text.
	void moveDescription(const sf::Vector2f& descriptionPosition);

private:
	/// Name graphical text.
	sf::Text _nameText;

	/// Cost graphical text.
	sf::Text _costText;

	/// Description graphical text.
	sf::Text _descriptionText;

	/// Text font.
	sf::Font _font;

	/// Position of the name relatively to the card.
	static const sf::Vector2f NAME_POSITION;

	/// Position of the cost relatively to the card.
	static const sf::Vector2f COST_POSITION;

	/// Width of the description frame.
	static constexpr float DESCRIPTION_WIDTH = 208.f;

	/// Character size.
	static constexpr std::size_t CHAR_SIZE = 20;

	/// Description character size.
	static constexpr std::size_t DESCRIPTION_CHAR_SIZE = 12;

	/// Path to the font.
	static constexpr char FONT_PATH[] = "../resources/client/FreeSans.otf";

	/// Path to the image of the back side of a card.
	static constexpr char BACK_IMAGE_PATH[] = "../resources/client/back.png";

	/// True when showing the front (with the image, title, ...), false when
	/// showing the back.
	bool _showFront;

	/// Shape that will contains the image of the back side.
	sf::Sprite _backView;

	/// Texture of the back side.
	sf::Texture _backTexture;

	/// Adds newlines in the given string (that has to be the text that will be
	/// displayed in the description of the card) so that the width of the text
	/// does not exceed the width of the description frame.
	/// \param description The string that will be modified.
	void addNewlinesInDescription(std::string& description);

	/// Get the width of the given character in pixels if it is displayed in the
	/// description frame (so the font and the character size of the description
	/// are used in the computation.
	/// \param character The character to test.
	/// \return The width of the given character in pixels.
	inline float getDescriptionCharacterWidth(char character) const;
};



#endif  // CARD_GUI_CLIENT_HPP
