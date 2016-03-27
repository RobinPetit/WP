// std-C++ headers
#include <iostream>
// WizardPoker header
#include "client/Gui/SpellGui.hpp"

const sf::Vector2f SpellGui::SPELL_DESCRIPTION_POSITION{26.f, 243.f};

SpellGui::SpellGui(CardId id, const std::string& name, const std::string& description, int cost):
	CardGui(id, name, description, cost)
{
	moveDescription(SPELL_DESCRIPTION_POSITION);
}
