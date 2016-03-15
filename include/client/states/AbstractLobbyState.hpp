#ifndef _ABSTRACT_LOBBY_STATE_CLIENT_HPP
#define _ABSTRACT_LOBBY_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include <string>

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractLobbyState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractLobbyState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		/////////// attributes
		static const std::string _quitString;

		/////////// private methods
		void quit();
		void startGame();
};

#endif// _ABSTRACT_LOBBY_STATE_CLIENT_HPP
