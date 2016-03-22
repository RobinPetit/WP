#ifndef _TERMINAL_ACHIEVEMENT_STATE_CLIENT_HPP
#define _TERMINAL_ACHIEVEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "common/Database.hpp"  // Ladder
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractAchievementState.hpp"

/// Final class for the achievements in the terminal.
class TerminalAchievementState : public TerminalAbstractState, public AbstractAchievementState
{
	public:
		/// Constructor.
		TerminalAchievementState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;
};

#endif// _TERMINAL_ACHIEVEMENT_STATE_CLIENT_HPP
