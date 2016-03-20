
#ifndef SPELLGUI_HPP
#define SPELLGUI_HPP

#include "CardGUI.hpp"

class SpellGUI: public CardGUI
{
public:
	/// Constructor
	SpellGUI(std::string cardIdentifier, int cost, std::vector<EffectParamsCollection> effects) // Taking only the required informations for the GUI

};

#endif //SPELLGUI_HPP

