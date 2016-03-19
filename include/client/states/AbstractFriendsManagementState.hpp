#ifndef _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the friends management state.
class AbstractFriendsManagementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractFriendsManagementState(Context& context);

	protected:
		void backMainMenu();

		void treatRequests();

		virtual void treatIndividualRequest(const Friend& friendRequest) = 0;
};

#endif// _ABSTRACT_FRIENDS_MANAGEMENT_STATE_CLIENT_HPP
