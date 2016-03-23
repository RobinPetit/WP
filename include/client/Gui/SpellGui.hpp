#ifndef SPELL_GUI_CLIENT_HPP
#define SPELL_GUI_CLIENT_HPP

#include "client/Gui/CardGui.hpp"

class SpellGui: public CardGui
{
public:
	/// Constructor
	SpellGui(std::string cardIdentifier, int cost, std::vector<EffectParamsCollection> effects) // Taking only the required informations for the GUI

};

#endif  // SPELL_GUI_CLIENT_HPP
