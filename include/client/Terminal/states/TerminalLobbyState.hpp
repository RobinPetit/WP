#ifndef _TERMINAL_LOBBY_STATE_CLIENT_HPP
#define _TERMINAL_LOBBY_STATE_CLIENT_HPP

#include "client/Terminal/TerminalAbstractState.hpp"
#include <string>

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class TerminalLobbyState : public TerminalAbstractState
{
	public:
		/// Constructor.
		TerminalLobbyState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		/////////// attributes
		static const std::string _quitString;

		/////////// private methods
		void quit();
		void startGame();
};

#endif// _TERMINAL_LOBBY_STATE_CLIENT_HPP
