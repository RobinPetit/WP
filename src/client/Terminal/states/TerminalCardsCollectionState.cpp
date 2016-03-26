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
	std::cout << "Here are your cards:\n";
	for(const auto& card : _cardsCollection)
		// The title of the card should be displayed rather than its ID
		std::cout << "* " << card << "\n";
	std::cout << std::string(40, '*') << "\n";

	// Display the actions
	TerminalAbstractState::display();
}
