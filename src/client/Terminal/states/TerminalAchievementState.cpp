// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalAchievementState.hpp"

TerminalAchievementState::TerminalAchievementState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractAchievementState(context)
{
	addAction("Back to main menu", &TerminalAchievementState::backMainMenu);
}

void TerminalAchievementState::display()
{
	displaySeparator("Achievements");
	for (std::size_t i=0; i<_achievements.size(); i++)
	{
		displayEntry(_achievements.at(i).getPrettyName());
		displayEntry(_achievements.at(i).getDescription(), ' ');
	}
	TerminalAbstractState::display();
}
