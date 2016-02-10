#include <iostream>
#include "client/states/LadderState.hpp"

LadderState::LadderState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Back to main menu", &LadderState::backMainMenu);
}

void LadderState::display()
{
    std::cout << "Here is the ladder:\n";

    // Display the actions
    AbstractState::display();
}

void LadderState::backMainMenu()
{
    stackPop();
}


