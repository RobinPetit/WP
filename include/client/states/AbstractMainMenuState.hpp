#ifndef _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP
#define _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the main menu state.
class AbstractMainMenuState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractMainMenuState(Context& context);

	protected:
		void logOut();
		void quit();
};

#endif// _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP
