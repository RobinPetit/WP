#include <iostream>
#include "client/states/LadderState.hpp"

LadderState::LadderState(StateStack& stateStack):
	AbstractState(stateStack)
{
	addAction("Back to main menu", &LadderState::backMainMenu);
	// Get the list of user from the server...
	_ladder[0] = {"Emile", 0, 1};
	_ladder[1] = {"Marc-Henry", 0, 0};
	_ladder[2] = {"Sigmund", 1, 0};
	_ladder[3] = {"Jean-jacques", 1, 1};
	_ladder[4] = {"Bertrand-Ernest", 10, 9};
	_ladder[5] = {"Rodalf", 6, 6};
	_ladder[6] = {"Björn", 666, 666};
	_ladder[7] = {"Sylvain", 4, 9};
	_ladder[8] = {"Bernard", 1, 10};
	_ladder[9] = {"Serge", 3, 1};
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


