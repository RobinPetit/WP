#include <iostream>
#include "client/states/MainMenuState.hpp"
#include "client/states/HomeState.hpp"

HomeState::HomeState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Connect with your account", &HomeState::connect);
    addAction("Create an account", &HomeState::createAccount);
    addAction("Quit", &HomeState::quit);
}

void HomeState::display()
{
    std::cout << "Welcome to Wizard Poker!\n";

    // Display the actions
    AbstractState::display();
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

void HomeState::quit()
{
	stackClear();
}
