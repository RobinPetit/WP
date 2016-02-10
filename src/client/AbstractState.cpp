#include <iostream>
#include "client/AbstractState.hpp"

AbstractState::AbstractState(StateStack& stateStack):
    _stateStack{stateStack}
{
}

void AbstractState::display()
{
    unsigned int i{0};
    for(const auto& pair : _actions)
        std::cout << ++i << ". " << pair.first << "\n";
}

void AbstractState::handleInput(const std::string& input)
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

void AbstractState::stackPop()
{
    _stateStack.pop();
}

void AbstractState::stackClear()
{
    _stateStack.clear();
}


