#include <iostream>
#include "client/states/FriendsManagementState.hpp"

FriendsManagementState::FriendsManagementState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Back to main menu", &FriendsManagementState::backMainMenu);
}

void FriendsManagementState::display()
{
    std::cout << "Here are your friends:\n";

    // Display the actions
    AbstractState::display();
}

void FriendsManagementState::backMainMenu()
{
    stackPop();
}


