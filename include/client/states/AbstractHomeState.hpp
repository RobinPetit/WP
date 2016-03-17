#ifndef _ABSTRACT_HOME_STATE_CLIENT_HPP
#define _ABSTRACT_HOME_STATE_CLIENT_HPP

#include <string>
#include <utility>
// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the home state.
class AbstractHomeState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractHomeState(StateStack& stateStack, Client& client);

	protected:
		void quit();
};

#endif// _ABSTRACT_HOME_STATE_CLIENT_HPP
