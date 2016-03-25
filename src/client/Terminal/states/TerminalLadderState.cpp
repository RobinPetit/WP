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
	unsigned i{0};
	for(const auto& ladderEntry : _ladder)
		std::cout << ++i << ". " << ladderEntry.name
		          << " (" << ladderEntry.victories + 1
		          << "/" << (ladderEntry.defeats + 1)
		          << ")\n";
	std::cout << std::string(40, '*') << "\n";
	// Display the actions
	TerminalAbstractState::display();
}
