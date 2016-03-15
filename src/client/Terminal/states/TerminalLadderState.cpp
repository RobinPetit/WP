// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/TerminalLadderState.hpp"

TerminalLadderState::TerminalLadderState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &TerminalLadderState::backMainMenu);
	// Sort the ladder according to user's win games/played games ratio
	// This sorting criterion can really be improved
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
	std::sort(_ladder.begin(), _ladder.end(), [](const LadderEntry& lhs, const LadderEntry& rhs)
	{
		if(lhs.victories + lhs.defeats == 0)
			return false;
		else if(rhs.victories + rhs.defeats == 0)
			return true;
		else
			return (static_cast<double>(lhs.victories) / static_cast<double>(lhs.victories + lhs.defeats))
					> (static_cast<double>(rhs.victories) / static_cast<double>(rhs.victories + rhs.defeats));
	});
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
	AbstractState::display();
}

void TerminalLadderState::backMainMenu()
{
	stackPop();
}
