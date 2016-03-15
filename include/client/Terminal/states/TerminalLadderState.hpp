#ifndef _TERMINAL_LADDER_STATE_CLIENT_HPP
#define _TERMINAL_LADDER_STATE_CLIENT_HPP

// WizardPoker headers
#include "common/Database.hpp"  // Ladder
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractLadderState.hpp"

/// Final class for the ladder in the terminal.
class TerminalLadderState : public TerminalAbstractState, public AbstractLadderState
{
	public:
		/// Constructor.
		TerminalLadderState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;
};

#endif// _TERMINAL_LADDER_STATE_CLIENT_HPP
