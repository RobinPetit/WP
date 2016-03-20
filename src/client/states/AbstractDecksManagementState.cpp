// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/AbstractDecksManagementState.hpp"
#include "client/StateStack.hpp"

AbstractDecksManagementState::AbstractDecksManagementState(Context& context):
	AbstractState(context)
{
	try
	{
		_decks = _context.client->getDecks();
		_cardsCollection = _context.client->getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void AbstractDecksManagementState::backMainMenu()
{
	_context.stateStack->pop();
}
