#ifndef CREATURE_GUI_CLIENT_HPP
#define CREATURE_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"

class CreatureGui: public CardGui
{
public:
	/// Constructor.
	CreatureGui(const std::string& name, const std::string& description, int cost, int attack, int health, int shield, int shieldType);

	/// Setters
	void setAttack(int value);
	void setHealth(int value);
	void setShield(int value);

	/// Draw the object to a render target.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	int _shieldType;// TODO use this

	sf::Text _attackText;
	sf::Text _healthText;
	sf::Text _shieldText;

	static const sf::Vector2f ATTACK_POSITION;
	static const sf::Vector2f HEALTH_POSITION;
	static const sf::Vector2f SHIELD_POSITION;
};

#endif  // CREATURE_GUI_CLIENT_HPP
