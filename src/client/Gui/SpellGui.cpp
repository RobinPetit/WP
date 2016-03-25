// std-C++ headers
#include <iostream>
// WizardPoker header
#include "client/Gui/SpellGui.hpp"

const sf::Vector2f SpellGui::SPELL_DESCRIPTION_POSITION{26.f, 243.f};
constexpr char SpellGui::FRONT_IMAGE_PATH[];

SpellGui::SpellGui(const std::string& name, const std::string& description, int cost):
	CardGui(FRONT_IMAGE_PATH, name, description, cost)
{
	moveDescription(SPELL_DESCRIPTION_POSITION);
}
