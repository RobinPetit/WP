#ifndef _TERMINAL_LOBBY_STATE_CLIENT_HPP
#define _TERMINAL_LOBBY_STATE_CLIENT_HPP

// Standard headers
#include <string>
// Wizard Poker headers
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractLobbyState.hpp"

/// Cannot be more explicit.
class TerminalLobbyState : public TerminalAbstractState, public AbstractLobbyState
{
	public:
		/// Constructor.
		TerminalLobbyState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		static const std::string _quitString;

		void startGame();
};

#endif// _TERMINAL_LOBBY_STATE_CLIENT_HPP
