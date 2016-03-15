#ifndef _CARDS_COLLECTION_STATE_CLIENT_HPP
#define _CARDS_COLLECTION_STATE_CLIENT_HPP

#include "client/Terminal/TerminalAbstractState.hpp"
#include "common/CardsCollection.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class TerminalCardsCollectionState : public TerminalAbstractState
{
	public:
		/// Constructor.
		TerminalCardsCollectionState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void backMainMenu();
		CardsCollection _cardsCollection;
};

#endif  // _CARDS_COLLECTION_STATE_CLIENT_HPP
