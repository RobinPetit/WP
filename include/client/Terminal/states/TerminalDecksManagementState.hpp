#ifndef _TERMINAL_DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _TERMINAL_DECKS_MANAGEMENT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Terminal/TerminalAbstractState.hpp"
#include "client/states/AbstractDecksManagementState.hpp"
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"

/// Final class for decks management in the terminal.
class TerminalDecksManagementState : public TerminalAbstractState, public AbstractDecksManagementState
{
	public:
		/// Constructor.
		TerminalDecksManagementState(Context& context);

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
};

#endif// _TERMINAL_DECKS_MANAGEMENT_STATE_CLIENT_HPP
