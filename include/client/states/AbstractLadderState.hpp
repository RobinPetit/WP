#ifndef _ABSTRACT_LADDER_STATE_CLIENT_HPP
#define _ABSTRACT_LADDER_STATE_CLIENT_HPP

// Wizard Poker headers
#include "common/Database.hpp"  // Ladder
#include "client/AbstractState.hpp"

/// Abstract version of the ladder state.
class AbstractLadderState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractLadderState(StateStack& stateStack, Client& client);

	protected:
		void backMainMenu();

		Ladder _ladder;
};

#endif// _ABSTRACT_LADDER_STATE_CLIENT_HPP
