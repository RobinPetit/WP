#ifndef _TERMINAL_HOME_STATE_CLIENT_HPP
#define _TERMINAL_HOME_STATE_CLIENT_HPP

// std-C++ headers
#include <string>
#include <utility>
// WizardPoker headers
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractHomeState.hpp"

/// Final class for the home in the terminal.
class TerminalHomeState : public TerminalAbstractState, public AbstractHomeState
{
	public:
		/// Constructor.
		TerminalHomeState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void connect();
		void createAccount();
		static std::pair<std::string, std::string> askIdentifiers();
};

#endif// _TERMINAL_HOME_STATE_CLIENT_HPP
