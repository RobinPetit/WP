#ifndef _MAIN_MENU_STATE_CLIENT_HPP
#define _MAIN_MENU_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class MainMenuState : public AbstractState
{
	public:
		/// Constructor.
		MainMenuState(StateStack& stateStack);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void findGame();
		void manageDecks();
		void seeCollection();
		void seeLadder();
		void manageFriends();
		void logOut();
		void quit();
};

#endif// _MAIN_MENU_STATE_CLIENT_HPP

