#ifndef _GUI_LOBBY_STATE_CLIENT_HPP
#define _GUI_LOBBY_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractLobbyState.hpp"
// External headers
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/VerticalLayout.hpp>

/// Final class for the lobby with the GUI.
class GuiLobbyState : public GuiAbstractState, public AbstractLobbyState
{
	public:
		/// Constructor.
		GuiLobbyState(Context& context);

	private:
		////////// Attributes
		const std::vector<ButtonData<GuiLobbyState>> _buttons;

		tgui::Button::Ptr _cancelButton;

		tgui::Label::Ptr _menuLabel;
		tgui::VerticalLayout::Ptr _buttonsLayout;

		////////// Methods

		void findAGame();
};

#endif  // _GUI_LOBBY_STATE_CLIENT_HPP
