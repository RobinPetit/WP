// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/AbstractCardsCollectionState.hpp"
#include "client/sockets/Client.hpp"
#include "client/StateStack.hpp"

AbstractCardsCollectionState::AbstractCardsCollectionState(Context& context):
	AbstractState(context)
{
	try
	{
		_cardsCollection = _context.client->getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void AbstractCardsCollectionState::backMainMenu()
{
	_context.stateStack->pop();
}
