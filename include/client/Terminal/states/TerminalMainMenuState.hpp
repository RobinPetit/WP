#ifndef _TERMINAL_MAIN_MENU_STATE_CLIENT_HPP
#define _TERMINAL_MAIN_MENU_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractMainMenuState.hpp"

/// Final class for the main menu in the terminal.
class TerminalMainMenuState : public TerminalAbstractState, public AbstractMainMenuState
{
	public:
		/// Constructor.
		TerminalMainMenuState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void findGame();
		void manageDecks();
		void seeCollection();
		void seeLadder();
		void manageFriends();
};

#endif// _TERMINAL_MAIN_MENU_STATE_CLIENT_HPP
