#ifndef _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

/// Abstract version of the decks management state.
class AbstractDecksManagementState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractDecksManagementState(Context& context);

	protected:
		void backMainMenu();
		std::vector<Deck> _decks;
		CardsCollection _cardsCollection;
};

#endif// _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
