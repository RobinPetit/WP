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
	displaySeparator("Cards");

	for(const auto& card : _cardsCollection)
		// The title of the card should be displayed rather than its ID
		std::cout << "* " << card << "\n";

	// Display the actions
	TerminalAbstractState::display();
}
