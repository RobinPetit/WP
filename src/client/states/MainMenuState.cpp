#include <iostream>
#include "client/states/MainMenuState.hpp"

MainMenuState::MainMenuState(StateStack& stateStack):
	AbstractState(stateStack)
{
}

void MainMenuState::display()
{
	std::cout << "Hi, this is the main menu, what do you want to do?\n";
	// Print the menu...
}

void MainMenuState::update()
{
}

void MainMenuState::handleEvent()
{
	std::string input;
	std::getline(std::cin, input);
	//Handle input...
}

