// std-C++ headers
#include <iostream>
#include <algorithm>
// WizardPoker headers
#include "client/states/AbstractAchievementState.hpp"

AbstractAchievementState::AbstractAchievementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	try
	{
		_achievements = _client.getAchievements();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Empty achievement list loaded.\n";
		return;
	}
}

void AbstractAchievementState::backMainMenu()
{
	stackPop();
}
