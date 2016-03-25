#ifndef _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP
#define _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"
#include "common/CardsCollection.hpp"

/// Abstract version of the card collection state.
class AbstractCardsCollectionState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractCardsCollectionState(Context& context);

	protected:
		CardsCollection _cardsCollection;

		void backMainMenu();
};

#endif  // _ABSTRACT_CARDS_COLLECTION_STATE_CLIENT_HPP
