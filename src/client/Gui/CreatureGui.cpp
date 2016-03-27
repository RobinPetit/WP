// std-C++ headers
#include <iostream>
// WizardPoker header
#include "client/Gui/CreatureGui.hpp"

const sf::Vector2f CreatureGui::HEALTH_POSITION{56.f, 217.f};
const sf::Vector2f CreatureGui::ATTACK_POSITION{129.f, 217.f};
const sf::Vector2f CreatureGui::SHIELD_POSITION{202.f, 217.f};
const sf::Vector2f CreatureGui::CREATURE_DESCRIPTION_POSITION{26.f, 273.f};

CreatureGui::CreatureGui(CardId id, const std::string& name, const std::string& description, int cost, int attack, int health, int shield, ShieldType shieldType):
	CardGui(id, name, description, cost),
	_shieldType{shieldType}
{
	moveDescription(CREATURE_DESCRIPTION_POSITION);

	setupText(_healthText, std::to_string(health), HEALTH_POSITION);
	_healthText.setColor(sf::Color(200, 0, 0));
	setupText(_attackText, std::to_string(attack), ATTACK_POSITION);
	setupText(_shieldText, std::to_string(shield), SHIELD_POSITION);
	switch(_shieldType)
	{
		case SHIELD_BLUE:
			_shieldText.setColor(sf::Color::Blue);
			break;

		case SHIELD_ORANGE:
			_shieldText.setColor(sf::Color(255, 160, 0));
			break;

		case SHIELD_LEGENDARY:
			_healthText.setColor(sf::Color(255, 200, 200));
			break;

		default:
			// By default, the text is black, this is fine
			break;
	}
}

void CreatureGui::setHealth(int value)
{
	_healthText.setString(std::to_string(value));
}

void CreatureGui::setAttack(int value)
{
	_attackText.setString(std::to_string(value));
}

void CreatureGui::setShield(int value)
{
	_shieldText.setString(std::to_string(value));
}

void CreatureGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	CardGui::draw(target, states);
	states.transform.combine(getTransform());
	target.draw(_healthText, states);
	target.draw(_attackText, states);
	target.draw(_shieldText, states);
}
