// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/AbstractCardsCollectionState.hpp"

AbstractCardsCollectionState::AbstractCardsCollectionState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	try
	{
		_cardsCollection = _client.getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void AbstractCardsCollectionState::backMainMenu()
{
	stackPop();
}
