#include <iostream>
#include "client/states/MainMenuState.hpp"

MainMenuState::MainMenuState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Find a game", &MainMenuState::findGame);
    addAction("Manage your decks", &MainMenuState::manageDecks);
    addAction("See your collection of cards", &MainMenuState::seeCollection);
    addAction("Go to chat", &MainMenuState::goToChat);
    addAction("Add a friend", &MainMenuState::addFriend);
    addAction("See ladder", &MainMenuState::seeLadder);
    addAction("Log out", &MainMenuState::logOut);
}

void MainMenuState::addAction(const std::string& actionName, void (MainMenuState::*method)())
{
    // emplace_back simply add an element to the end of the vector.
    // std::bind constructs a function that results in the call
    // of method with *this as implicit first argument.
    _actions.emplace_back(actionName, std::bind(method, *this));
}

void MainMenuState::display()
{
    // The menu is not that pretty, it can be improved
    std::cout << std::string(40, '*') << "\n";
    std::cout << "Main menu:\n";
    unsigned int i{0};
    // Display the actions
    for(const auto& pair : _actions)
        std::cout << ++i << ". " << pair.first << "\n";
    std::cout << std::string(40, '*') << "\n";
    std::cout << "What do you want to do? ";
}

void MainMenuState::handleEvent()
{
    try
    {
        // Get a number from the user input
        std::string input;
        std::getline(std::cin, input);
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

void MainMenuState::findGame()
{
    std::cout << "So, let's find an opponent...\n";
}

void MainMenuState::manageDecks()
{
    std::cout << "So, let's manage your decks...\n";
}

void MainMenuState::seeCollection()
{
    std::cout << "So, let's see your cards...\n";
}

void MainMenuState::goToChat()
{
    std::cout << "So, let's talk to someone's else...\n";
}

void MainMenuState::seeLadder()
{
    std::cout << "So, let's see the ladder...\n";
}

void MainMenuState::addFriend()
{
    std::cout << "So, let's meet new people...\n";
}

void MainMenuState::logOut()
{
    std::cout << "Bye!...\n";
    stackClear();
}


