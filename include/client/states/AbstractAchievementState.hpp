#ifndef _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the achievement state.
class AbstractAchievementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractAchievementState(StateStack& stateStack, Client& client);

	protected:
		void backMainMenu();

		//Ladder _ladder;
};

#endif  // _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP
