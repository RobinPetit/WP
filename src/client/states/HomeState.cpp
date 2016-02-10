#include <iostream>
#include "client/states/MainMenuState.hpp"
#include "client/states/HomeState.hpp"

HomeState::HomeState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Connect with your account", &HomeState::connect);
    addAction("Create an account", &HomeState::createAccount);
}

void HomeState::display()
{
    std::cout << "Welcome to Wizard Poker!\n";
    unsigned int i{0};
    // Display the actions
    for(const auto& pair : _actions)
        std::cout << ++i << ". " << pair.first << "\n";
    std::cout << "What do you want to do? ";
}

void HomeState::handleInput(const std::string& input)
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

void HomeState::connect()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);
	// Do connecting stuff here
	std::cout << "Hello " << userName << "!\n";
	stackPush<MainMenuState>();
}

void HomeState::createAccount()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);
	// Do subscribing stuff here
	std::cout << "Hello " << userName << "!\n";
	stackPush<MainMenuState>();
}


