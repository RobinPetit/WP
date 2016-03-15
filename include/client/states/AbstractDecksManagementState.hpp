#ifndef _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractDecksManagementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractDecksManagementState(StateStack& stateStack, Client& client);

	protected:
		void backMainMenu();
		std::vector<Deck> _decks;
		CardsCollection _cardsCollection;
};

#endif// _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
