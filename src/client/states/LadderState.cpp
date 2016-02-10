#include <iostream>
#include "client/states/LadderState.hpp"

LadderState::LadderState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Back to main menu", &LadderState::backMainMenu);
    // Get the list of user from the server...
    _ladder = {"Emile", "Marc-Henry", "Sigmund"};
}

void LadderState::display()
{
    std::cout << "Here is the ladder:\n";
    unsigned int i{0}
	for(const auto& user : _ladder)
		std::cout << ++i << ". " << user << "\n";

    // Display the actions
    AbstractState::display();
}

void LadderState::backMainMenu()
{
    stackPop();
}


