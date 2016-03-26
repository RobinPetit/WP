#ifndef CREATURE_GUI_CLIENT_HPP
#define CREATURE_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"
#include "common/CardData.hpp"

class CreatureGui: public CardGui
{
public:
	/// Constructor.
	CreatureGui(const std::string& name, const std::string& description, int cost, int attack, int health, int shield, ShieldType shieldType);

	/// Sets the health.
	void setHealth(int value);

	/// Sets the attack.
	void setAttack(int value);

	/// Sets the shield.
	void setShield(int value);

	/// Draws the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	/// Graphical text for the health.
	sf::Text _healthText;

	/// Graphical text for the attack.
	sf::Text _attackText;

	/// Graphical text for the shield.
	sf::Text _shieldText;

	/// Type of shield, determines the color of the shield text.
	ShieldType _shieldType;

	/// Position of the health text.
	static const sf::Vector2f HEALTH_POSITION;

	/// Position of the attack text.
	static const sf::Vector2f ATTACK_POSITION;

	/// Position of the shield text.
	static const sf::Vector2f SHIELD_POSITION;

	/// Position of the description relatively to the card.
	static const sf::Vector2f CREATURE_DESCRIPTION_POSITION;

	/// Path of the image of the front side for a creature.
	static constexpr char FRONT_IMAGE_PATH[] = "../resources/client/creatureFront.png";
};

#endif  // CREATURE_GUI_CLIENT_HPP
