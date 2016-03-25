#ifndef _TERMINAL_LOBBY_STATE_CLIENT_HPP
#define _TERMINAL_LOBBY_STATE_CLIENT_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractLobbyState.hpp"

/// Final class for the lobby in the terminal.
class TerminalLobbyState : public TerminalAbstractState, public AbstractLobbyState
{
	public:
		/// Constructor.
		TerminalLobbyState(Context& context);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		static const std::string _quitString;

		void findAGame();
};

#endif// _TERMINAL_LOBBY_STATE_CLIENT_HPP
