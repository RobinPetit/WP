#ifndef _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the friends management state.
class AbstractFriendsManagementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractFriendsManagementState(StateStack& stateStack, Client& client);

	protected:
		void backMainMenu();
};

#endif// _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP
