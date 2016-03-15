#ifndef _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP
#define _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

/// Abstract version of the main menu state.
class AbstractMainMenuState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractMainMenuState(StateStack& stateStack, Client& client);

	protected:
		void logOut();
		void quit();
};

#endif// _ABSTRACT_MAIN_MENU_STATE_CLIENT_HPP
