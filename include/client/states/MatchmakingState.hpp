#ifndef _MATCHMAKING_STATE_CLIENT_HPP
#define _MATCHMAKING_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class MatchmakingState : public AbstractState
{
	public:
		/// Constructor.
		MatchmakingState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void quit();

};

#endif// _MATCHMAKING_STATE_CLIENT_HPP

