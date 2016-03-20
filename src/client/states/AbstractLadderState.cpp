// std-C++ headers
#include <iostream>
#include <algorithm>
// WizardPoker headers
#include "client/states/AbstractLadderState.hpp"

AbstractLadderState::AbstractLadderState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	try
	{
		_ladder = _client.getLadder();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Empty ladder loaded.\n";
		return;
	}
	std::sort(_ladder.ladder.begin(), _ladder.ladder.begin() + _ladder.nbOfPlayers, [](const LadderEntry& lhs, const LadderEntry& rhs)
	{
		if(lhs.victories + lhs.defeats == 0)
			return false;
		else if(rhs.victories + rhs.defeats == 0)
			return true;
		else
			return (static_cast<double>(lhs.victories + 1) / (lhs.defeats + 1))
			        > (static_cast<double>(rhs.victories + 1) / (rhs.defeats + 1));
	});
}

void AbstractLadderState::backMainMenu()
{
	stackPop();
}
