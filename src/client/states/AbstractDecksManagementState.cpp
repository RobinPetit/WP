// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/AbstractDecksManagementState.hpp"
#include "common/CardData.hpp"

AbstractDecksManagementState::AbstractDecksManagementState(StateStack& stateStack, Client& client):
	AbstractState{stateStack, client}
{
	try
	{
		_decks = _client.getDecks();
		_cardsCollection = _client.getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void AbstractDecksManagementState::backMainMenu()
{
	stackPop();
}
