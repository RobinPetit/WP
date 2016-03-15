#ifndef _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include "common/CardsCollection.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractCardsCollectionState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractCardsCollectionState(StateStack& stateStack, Client& client);

	protected:
		CardsCollection _cardsCollection;

		void backMainMenu();
};

#endif  // _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP
