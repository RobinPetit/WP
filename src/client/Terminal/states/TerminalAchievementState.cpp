// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalAchievementState.hpp"

TerminalAchievementState::TerminalAchievementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	TerminalAbstractState(stateStack, client),
	AbstractAchievementState(stateStack, client)
{
	addAction("Back to main menu", &TerminalAchievementState::backMainMenu);
}

void TerminalAchievementState::display()
{
	std::cout << "Here are your achievements:\n";
	/*unsigned i{0};
	for(const auto& ladderEntry : _ladder)
		std::cout << ++i << ". " << ladderEntry.name
		          << " (" << ladderEntry.victories + 1
		          << "/" << (ladderEntry.defeats + 1)
		          << ")\n";
	std::cout << std::string(40, '*') << "\n";
	// Display the actions*/
	TerminalAbstractState::display();
}
