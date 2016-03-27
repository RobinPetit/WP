#ifndef SPELL_GUI_CLIENT_HPP
#define SPELL_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"

class SpellGui: public CardGui
{
public:
	/// Constructor
	SpellGui(CardId id, const std::string& name, const std::string& description, int cost);

private:
	/// Position of the description relatively to the card.
	static const sf::Vector2f SPELL_DESCRIPTION_POSITION;

};

#endif  // SPELL_GUI_CLIENT_HPP
