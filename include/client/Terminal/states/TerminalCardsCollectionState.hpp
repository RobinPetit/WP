#ifndef _TERMINAL_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _TERMINAL_CARDS_COLLECTION_STATE_CLIENT_HPP

#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractCardsCollectionState.hpp"

/// Cannot be more explicit.
class TerminalCardsCollectionState : public TerminalAbstractState, public AbstractCardsCollectionState
{
	public:
		/// Constructor.
		TerminalCardsCollectionState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;
};

#endif  // _TERMINAL_CARDS_COLLECTION_STATE_CLIENT_HPP
