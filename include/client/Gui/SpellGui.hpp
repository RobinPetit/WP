#ifndef SPELL_GUI_CLIENT_HPP
#define SPELL_GUI_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/CardGui.hpp"

class SpellGui: public CardGui
{
public:
	/// Constructor
	SpellGui(const std::string& name, const std::string& description, int cost);

};

#endif  // SPELL_GUI_CLIENT_HPP
