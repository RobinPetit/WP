#ifndef _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"
#include "common/Identifiers.hpp"

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

		/// Returns the cards collection in a vector, but with all the cards of
		/// the given deck stripped from it. In fact, this method returns only
		/// the cards that could be added to the given deck from the collection:
		/// if the deck contains 2 time the same card, this card won't be in
		/// the result, even if the user has this card 5 times, because he won't
		/// be able to add it one more time in the deck. This also applies if
		/// the deck contains 1 or 0 times a card.
		/// So the result will never contains more than 2 times a given card.
		std::vector<CardId> getCardsCollectionWithoutDeck(const Deck& deck) const;
};

#endif// _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
