#ifndef _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractDecksManagementState : public AbstractState
{
	public:
		/// Constructor.
		AbstractDecksManagementState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void displayDeck();
		void editDeck();
		// Edit deck submethods
		std::size_t askForReplacedCard(std::size_t deckIndex);
		cardId askForReplacingCard(std::size_t deckIndex);


		void createDeck();
		void deleteDeck();
		void backMainMenu();

		std::vector<Deck> _decks;
		CardsCollection _cardsCollection;
};

#endif// _ABSTRACT_DECKS_MANAGEMENT_STATE_CLIENT_HPP
