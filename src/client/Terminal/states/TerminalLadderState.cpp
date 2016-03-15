// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalLadderState.hpp"

TerminalLadderState::TerminalLadderState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractLadderState(context)
{
	addAction("Back to main menu", &TerminalLadderState::backMainMenu);
}

void TerminalLadderState::display()
{
	std::cout << "Here is the ladder:\n";
	unsigned int i{0U};
	for(const auto& ladderEntry : _ladder)
		std::cout << ++i << ". " << ladderEntry.name
				<< " (" << ladderEntry.victories
				<< "/" << (ladderEntry.victories + ladderEntry.defeats)
				<< ")\n";
	std::cout << std::string(40, '*') << "\n";
	// Display the actions
	TerminalAbstractState::display();
}
