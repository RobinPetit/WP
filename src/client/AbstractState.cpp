#include <iostream>
#include <cassert>
#include "client/AbstractState.hpp"

AbstractState::AbstractState(StateStack& stateStack, Client& client):
	_stateStack{stateStack},
	_client{client}
{

}

void AbstractState::display()
{
	for(size_t i{1}; i < _actions.size(); ++i)
		std::cout << i << ". " << _actions[i].first << "\n";
	assert(_actions.size() > 0);
	// Display the menu entry 0 (which should be 'quit' or something like this) at last
	// because this is strange to have 'quit' as first possibility in a menu
	std::cout << "0. " << _actions[0].first << "\n";
}

void AbstractState::handleInput(const std::string& input)
{
	try
	{
		// Get a number from the user input
		const int intInput{std::stoi(input)};
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


