#ifndef _DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _DECKS_MANAGEMENT_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class DecksManagementState : public AbstractState
{
	public:
		/// Constructor.
		DecksManagementState(StateStack& stateStack);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void displayDeck();
		void editDeck();
		// Edit deck submethods
		std::size_t askForReplacedCard(std::size_t deckIndex);
		Card::ID askForReplacingCard(std::size_t deckIndex);


		void createDeck();
		void deleteDeck();
		void backMainMenu();
		static std::size_t askForNumber(std::size_t from, std::size_t to);

		std::vector<Deck> _decks;
		CardsCollection _cardsCollection;
};

#endif// _DECKS_MANAGEMENT_STATE_CLIENT_HPP

