#include <iostream>
#include "client/states/CardsCollectionState.hpp"

CardsCollectionState::CardsCollectionState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Back to main menu", &CardsCollectionState::backMainMenu);
}

void CardsCollectionState::display()
{
    std::cout << "Here are your cards:\n";
    unsigned int i{0};
    // Display the actions
    for(const auto& pair : _actions)
        std::cout << ++i << ". " << pair.first << "\n";
    std::cout << "What do you want to do? ";
}

void CardsCollectionState::handleEvent(const std::string& input)
{
    try
    {
        // Get a number from the user input
        const int intInput{std::stoi(input) - 1};
        // Call the method at index intInput
        // std::vector::at throws std::out_of_range if intInput is out of bounds
        _actions.at(intInput).second();
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong input!\n";
    }
}

void CardsCollectionState::backMainMenu()
{
    stackPop();
}


