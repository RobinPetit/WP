#ifndef _GUI_HOME_STATE_CLIENT_HPP
#define _GUI_HOME_STATE_CLIENT_HPP

// External headers
#include <TGUI/TGUI.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractHomeState.hpp"

/// Final class for the home in the terminal.
class GuiHomeState : public GuiAbstractState, public AbstractHomeState
{
	public:
		/// Constructor.
		GuiHomeState(Context& context);

		/// Destructor. Deletes all widgets of this state.
		~GuiHomeState();

		/// Method called when another state is pushed on this one.
		/// By default, does nothing.
		/// Hides all the widgets.
		virtual void onPush() override;

		/// Method called when this state become the TOS again (e.g. this method
		/// is called on the HomeState instance when we log out from the main
		/// menu state).
		/// Shows all the widgets again.
		virtual void onPop() override;

	private:
		void connect();
		void createAccount();

		// Attributes
		tgui::Label::Ptr _titleLabel;
		tgui::Label::Ptr _userNameLabel;
		tgui::Label::Ptr _passwordLabel;
		tgui::EditBox::Ptr _userNameEditBox;
		tgui::EditBox::Ptr _passwordEditBox;
		tgui::Button::Ptr _connectButton;
		tgui::Button::Ptr _createAccountButton;
		tgui::Grid::Ptr _grid;
};

#endif  // _GUI_HOME_STATE_CLIENT_HPP
