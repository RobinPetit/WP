#ifndef ACHIEVEMENT_GUI_CLIENT_HPP
#define ACHIEVEMENT_GUI_CLIENT_HPP

// std-C++ headers
#include <string>
// External headers
#include <SFML/Graphics.hpp>

/// TODO: handle line wraps in the description field.
/// Graphical representation of an achievement. It behaves like other SFML graphicals classes
class AchievementGui : public sf::Drawable, public sf::Transformable
{
public:
	/// Constructor.
	AchievementGui(const std::string& name, const std::string& description, bool isUnlocked);

	/// Destructor.
	virtual ~AchievementGui() = default;

	/// Returns the size of an achievement. Since all achievements have the same size, this
	/// method is static.
	/// \return the size of a card.
	static sf::Vector2f getSize();

	/// Draws the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	/// True when achievement is unlocked
	bool _isUnlocked;

	/// The background rectangle, with a color
	sf::RectangleShape _bgRectangle;

	/// The main shape that contains the texture.
	sf::Sprite _imageSprite;

	/// The texture displayed for the achievement
	sf::Texture _imageTexture;

	/// Name graphical text.
	sf::Text _nameText;

	/// Description graphical text.
	sf::Text _descriptionText;

	/// Text font.
	sf::Font _font;

	/// The global size of the achievement container, in pixels.
	static const sf::Vector2f SIZE;

	/// The global size of the achievement image, in pixels.
	static const sf::Vector2f IMAGE_SIZE;

	/// Position of the image relatively to the container
	static const sf::Vector2f IMAGE_POSITION;

	/// Position of the name relatively to the container
	static const sf::Vector2f NAME_POSITION;

	/// Position of the description relatively to the container
	static const sf::Vector2f DESCRIPTION_POSITION;

	/// Character size.
	static constexpr std::size_t NAME_CHAR_SIZE = 20;

	/// Description character size.
	static constexpr std::size_t DESCRIPTION_CHAR_SIZE = 14;

	/// Path to the font.
	static constexpr char FONT_PATH[] = "../resources/client/FreeSans.otf";

	/// Path to the images of the locked and unlocked achievements
	static constexpr char IMAGE_LOCKED_PATH[] = "../resources/client/achievement_locked.png";
	static constexpr char IMAGE_UNLOCKED_PATH[] = "../resources/client/achievement_unlocked.png";


	/// Ease of use method. It configures the card with the right font, position
	/// , text size and color.
	void setupText(sf::Text& text, std::string string, const sf::Vector2f& position) const;
};



#endif  // ACHIEVEMENT_GUI_CLIENT_HPP
