#ifndef _LADDER_STATE_CLIENT_HPP
#define _LADDER_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class LadderState : public AbstractState
{
	public:
		/// An entry in the ladder. There is no need for further data about the
		/// users shown in the list.
		struct LadderEntry
		{
			std::string _userName;
			unsigned int _wonGames;
			unsigned int _loseGames;
		};

		/// Constructor.
		LadderState(StateStack& stateStack);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void backMainMenu();

		std::vector<LadderEntry> _ladder;
};

#endif// _LADDER_STATE_CLIENT_HPP

