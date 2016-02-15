#ifndef _DECKS_MANAGEMENT_STATE_CLIENT_HPP
#define _DECKS_MANAGEMENT_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

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
		// \TODO write DisplayDeckState, EditDeckState and CreateDeckState
		void displayDeck();
		void editDeck();
		void createDeck();
		void deleteDeck();
		void backMainMenu();
		static int askForNumber(int from, int to);

		std::vector<std::string> _decks;// Use the class Deck as soon as possible
};

#endif// _DECKS_MANAGEMENT_STATE_CLIENT_HPP

