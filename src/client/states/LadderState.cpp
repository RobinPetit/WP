#include <iostream>
#include "client/states/LadderState.hpp"

LadderState::LadderState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &LadderState::backMainMenu);
	// Get the list of user from the server...
	_ladder = {{"Emile", 0, 1}, {"Marc-Henry", 0, 0}, {"Sigmund", 1, 0},
			{"Jean-jacques", 1, 1}, {"Bertrand-Ernest", 10, 9}, {"Rodalf", 6, 6},
			{"Björn", 666, 666}, {"Sylvain", 4, 9}, {"Bernard", 1, 10},
			{"Serge", 3, 1}};
	// Sort the ladder according to user's win games/played games ratio
	// This sorting criterion can really be improved
	std::sort(_ladder.begin(), _ladder.end(), [](const LadderEntry& lhs, const LadderEntry& rhs)
	{
		if(lhs._wonGames + lhs._loseGames == 0)
			return false;
		else if(rhs._wonGames + rhs._loseGames == 0)
			return true;
		else
			return (static_cast<double>(lhs._wonGames) / static_cast<double>(lhs._wonGames + lhs._loseGames))
					> (static_cast<double>(rhs._wonGames) / static_cast<double>(rhs._wonGames + rhs._loseGames));
	});
}

void LadderState::display()
{
	std::cout << "Here is the ladder:\n";
	unsigned int i{0U};
	for(const auto& ladderEntry : _ladder)
		std::cout << ++i << ". " << ladderEntry._userName
				<< " (" << ladderEntry._wonGames
				<< "/" << (ladderEntry._wonGames + ladderEntry._loseGames)
				<< ")\n";
	std::cout << std::string(40, '*') << "\n";
	// Display the actions
	AbstractState::display();
}

void LadderState::backMainMenu()
{
	stackPop();
}
