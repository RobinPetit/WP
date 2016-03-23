#ifndef _GUI_LOBBY_STATE_CLIENT_HPP
#define _GUI_LOBBY_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractLobbyState.hpp"

/// Final class for the lobby with the GUI.
class GuiLobbyState : public GuiAbstractState, public AbstractLobbyState
{
	public:
		/// Constructor.
		GuiLobbyState(Context& context);
};

#endif  // _GUI_LOBBY_STATE_CLIENT_HPP
