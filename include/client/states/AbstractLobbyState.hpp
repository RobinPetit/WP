#ifndef _ABSTRACT_LOBBY_STATE_CLIENT_HPP
#define _ABSTRACT_LOBBY_STATE_CLIENT_HPP

// Standard headers
#include <string>
// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the lobby state.
class AbstractLobbyState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractLobbyState(StateStack& stateStack, Client& client);

	protected:
		void quit();
};

#endif// _ABSTRACT_LOBBY_STATE_CLIENT_HPP
