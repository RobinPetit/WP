#ifndef _GUI_HOME_STATE_CLIENT_HPP
#define _GUI_HOME_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"
#include "client/states/AbstractHomeState.hpp"

/// Final class for the home in the terminal.
class GuiHomeState : public GuiAbstractState, public AbstractHomeState
{
	public:
		/// Constructor.
		GuiHomeState(Context& context);

	private:
		void connect();
		void createAccount();
};

#endif  // _GUI_HOME_STATE_CLIENT_HPP
