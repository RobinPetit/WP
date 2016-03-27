#ifndef _ABSTRACT_LOBBY_STATE_CLIENT_HPP
#define _ABSTRACT_LOBBY_STATE_CLIENT_HPP

// Standard headers
#include <string>
// WizardPoker headers
#include "client/AbstractState.hpp"
#include "client/AbstractGame.hpp"

/// Abstract version of the lobby state.
class AbstractLobbyState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractLobbyState(Context& context);

	protected:
		void quit();

		void startGame(AbstractGame& game);
};

#endif// _ABSTRACT_LOBBY_STATE_CLIENT_HPP
