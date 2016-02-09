#include <iostream>
#include "client/states/MainMenuState.hpp"

MainMenuState::MainMenuState(StateStack& stateStack):
	AbstractState(stateStack)
{
	addAction("Find a game", &MainMenuState::findGame);
	addAction("Manage your decks", &MainMenuState::manageDecks);
	addAction("Change your account settings", &MainMenuState::changeAccountSettings);
	addAction("Log out", &MainMenuState::logOut);
}

void MainMenuState::addAction(const std::string& actionName, void (MainMenuState::*method)())
{
	_actions.emplace_back(actionName, std::bind(method, *this));
}

void MainMenuState::display()
{
	std::cout << std::string(40, '*') << "\n";
	std::cout << "Main menu:\n";
	unsigned int i{0};
	for(const auto& pair : _actions)
		std::cout << ++i << ". " << pair.first << "\n";
	std::cout << std::string(40, '*') << "\n";
	std::cout << "What do you want to do? ";
}

void MainMenuState::handleEvent()
{
	std::string input;
	std::getline(std::cin, input);
	const int intInput{std::stoi(input) - 1};
	if(intInput >= 0 and intInput < _actions.size())
		// Call the method at index intInput
		_actions[intInput].second();
	else
		std::cout << "Wrong input!\n";
}

void MainMenuState::findGame()
{
	std::cout << "So, let's find an opponent...\n";
}

void MainMenuState::manageDecks()
{
	std::cout << "So, let's manage your decks...\n";
}

void MainMenuState::changeAccountSettings()
{
	std::cout << "So, let's change some settings...\n";
}

void MainMenuState::logOut()
{
	std::cout << "Bye!...\n";
	stackClear();
}


