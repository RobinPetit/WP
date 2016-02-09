#ifndef _MAIN_MENU_STATE_CLIENT_HPP
#define _MAIN_MENU_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include <vector>
#include <utility>
#include <string>
#include <functional>

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

        /// The event handling function.
		virtual void handleEvent() override;

	private:
		void addAction(const std::string& actionName, void (MainMenuState::*method)());
		void findGame();
		void manageDecks();
		void changeAccountSettings();
		void logOut();

		std::vector<std::pair<std::string, std::function<void()>>> _actions;
};

#endif// _MAIN_MENU_STATE_CLIENT_HPP

