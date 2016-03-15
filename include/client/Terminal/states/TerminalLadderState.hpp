#ifndef _TERMINAL_LADDER_STATE_CLIENT_HPP
#define _TERMINAL_LADDER_STATE_CLIENT_HPP

#include "common/Database.hpp"// Ladder
#include "client/Terminal/TerminalAbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class TerminalLadderState : public TerminalAbstractState
{
	public:
		/// Constructor.
		TerminalLadderState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void backMainMenu();

		Ladder _ladder;
};

#endif// _TERMINAL_LADDER_STATE_CLIENT_HPP
