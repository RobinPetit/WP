#ifndef _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractDecksManagementState.hpp"

/// Final class for decks management with the GUI.
class GuiDecksManagementState : public GuiAbstractState, public AbstractDecksManagementState
{
	public:
		/// Constructor.
		GuiDecksManagementState(Context& context);
};

#endif  // _GUI_DECKS_MANAGEMENT_STATE_CLIENT_HPP
