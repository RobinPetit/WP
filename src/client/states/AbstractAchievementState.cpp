// std-C++ headers
#include <iostream>
#include <algorithm>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/StateStack.hpp"
#include "client/states/AbstractAchievementState.hpp"

AbstractAchievementState::AbstractAchievementState(Context& context):
	AbstractState(context)
{
	try
	{
		_achievements = _context.client->getAchievements();
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
	_context.stateStack->pop();
}
