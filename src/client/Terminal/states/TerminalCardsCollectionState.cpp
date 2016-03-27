// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalCardsCollectionState.hpp"

TerminalCardsCollectionState::TerminalCardsCollectionState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractCardsCollectionState(context)
{
	addAction("Back to main menu", &TerminalCardsCollectionState::backMainMenu);
}

void TerminalCardsCollectionState::display()
{
	displaySeparator(std::string("Unlocked Cards (") + std::to_string(_cardsCollection.getSize()) + ")");

	// Displays simple informations about a card vector
	for (auto& cardId : _cardsCollection)
		displayCard(cardId);

	// Display the actions
	TerminalAbstractState::display();
}
