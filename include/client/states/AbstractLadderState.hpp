#ifndef _ABSTRACT_LADDER_STATE_CLIENT_HPP
#define _ABSTRACT_LADDER_STATE_CLIENT_HPP

// WizardPoker headers
#include "common/Ladder.hpp"
#include "client/AbstractState.hpp"

/// Abstract version of the ladder state.
class AbstractLadderState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractLadderState(Context& context);

	protected:
		void backMainMenu();

		Ladder _ladder;
};

#endif  // _ABSTRACT_LADDER_STATE_CLIENT_HPP
