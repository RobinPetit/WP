#ifndef _ABSTRACT_LADDER_STATE_CLIENT_HPP
#define _ABSTRACT_LADDER_STATE_CLIENT_HPP

#include "common/Database.hpp"// Ladder
#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractLadderState : public AbstractState
{
	public:
		/// Constructor.
		AbstractLadderState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void backMainMenu();

		Ladder _ladder;
};

#endif// _ABSTRACT_LADDER_STATE_CLIENT_HPP
