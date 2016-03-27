#ifndef _GUI_HOME_STATE_CLIENT_HPP
#define _GUI_HOME_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractHomeState.hpp"

/// Final class for the home with the GUI.
class GuiHomeState : public GuiAbstractState, public AbstractHomeState
{
	public:
		/// Constructor.
		GuiHomeState(Context& context);

	private:
		void connect();
		void createAccount();

		// Attributes
		tgui::Label::Ptr _userNameLabel;
		tgui::Label::Ptr _passwordLabel;
		tgui::EditBox::Ptr _userNameEditBox;
		tgui::EditBox::Ptr _passwordEditBox;
		tgui::Button::Ptr _connectButton;
		tgui::Button::Ptr _createAccountButton;
		tgui::Grid::Ptr _grid;
		tgui::Grid::Ptr _gridBottom;
};

#endif  // _GUI_HOME_STATE_CLIENT_HPP
