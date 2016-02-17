#include <iostream>
#include "client/states/CardsCollectionState.hpp"

CardsCollectionState::CardsCollectionState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &CardsCollectionState::backMainMenu);
	// Get the list of cards from the server...
}

void CardsCollectionState::display()
{
	std::cout << "Here are your cards:\n";
	for(const auto& card : _cardsCollection)
		// The title of the card should be displayed rather than its ID
		std::cout << "* " << card << "\n";
	std::cout << std::string(40, '*') << "\n";

	// Display the actions
	AbstractState::display();
}

void CardsCollectionState::backMainMenu()
{
	stackPop();
}


