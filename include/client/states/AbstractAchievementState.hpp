#ifndef _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"
#include "client/ClientAchievement.hpp"

/// Abstract version of the achievement state.
class AbstractAchievementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractAchievementState(StateStack& stateStack, Client& client);

	protected:
		void backMainMenu();

		ClientAchievementList _achievements;
};

#endif  // _ABSTRACT_ACHIEVEMENT_STATE_CLIENT_HPP
