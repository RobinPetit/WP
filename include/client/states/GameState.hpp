#ifndef _GAME_STATE_CLIENT_HPP
#define _GAME_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class GameState : public AbstractState
{
	public:
		/// Constructor.
		GameState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void quit();

};

#endif// _GAME_STATE_CLIENT_HPP

