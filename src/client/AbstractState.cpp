#include <iostream>
#include <cassert>
#include "client/AbstractState.hpp"

AbstractState::AbstractState(StateStack& stateStack, Client& client):
	_client{client},
	_stateStack{stateStack}
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
		waitForEnter();
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

void AbstractState::waitForEnter()
{
	std::cout << "Press Enter to continue...";
	std::string tmp;
	std::getline(std::cin, tmp);
}

std::size_t AbstractState::askForNumber(std::size_t from, std::size_t to)
{
	std::string input;
	do
	{
		std::getline(std::cin, input);
	} while(input == "");
	// Get a number from the user input
	const std::size_t intInput{std::stoul(input)};
	if(intInput < from or intInput >= to)
		throw std::out_of_range("Index out of range.");
	return intInput;
}
