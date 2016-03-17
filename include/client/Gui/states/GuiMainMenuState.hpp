#ifndef _GUI_MAIN_MENU_STATE_CLIENT_HPP
#define _GUI_MAIN_MENU_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractMainMenuState.hpp"

/// Main menu in GUI Interface
class GuiMainMenuState: public GuiAbstractState, public AbstractMainMenuState
{
	public:
		/// Constructor
		GuiMainMenuState(Context& context);
};

#endif // _GUI_MAIN_MENU_STATE_CLIENT_HPP
