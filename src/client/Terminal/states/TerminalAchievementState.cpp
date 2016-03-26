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
	for (std::size_t i=0; i<_achievements.size(); i++)
	{
		std::cout << " " << _achievements.at(i).getPrettyName() << std::endl;
		std::cout << "   " << _achievements.at(i).getDescription() << std::endl;
	}
	TerminalAbstractState::display();
}
