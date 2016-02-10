#include <iostream>
#include "client/states/CardsCollectionState.hpp"
#include "client/states/DecksManagementState.hpp"
#include "client/states/FriendsManagementState.hpp"
#include "client/states/LadderState.hpp"
#include "client/states/MainMenuState.hpp"

MainMenuState::MainMenuState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Find a game", &MainMenuState::findGame);
    addAction("Manage your decks", &MainMenuState::manageDecks);
    addAction("See your collection of cards", &MainMenuState::seeCollection);
    addAction("Manage your friends", &MainMenuState::manageFriends);
    addAction("See the ladder", &MainMenuState::seeLadder);
    addAction("Log out", &MainMenuState::logOut);
}

void MainMenuState::display()
{
    // The menu is not that pretty, it can be improved
    std::cout << std::string(40, '*') << "\n";
    std::cout << "Main menu:\n";

    // Display the actions
    AbstractState::display();
}

void MainMenuState::findGame()
{
    std::cout << "So, let's find an opponent...\n";
}

void MainMenuState::manageDecks()
{
    std::cout << "So, let's manage your decks...\n";
    stackPush<DecksManagementState>();
}

void MainMenuState::seeCollection()
{
    std::cout << "So, let's see your cards...\n";
    stackPush<CardsCollectionState>();
}

void MainMenuState::seeLadder()
{
    std::cout << "So, let's see the ladder...\n";
    stackPush<LadderState>();
}

void MainMenuState::manageFriends()
{
    std::cout << "So, let's meet new people...\n";
    stackPush<FriendsManagementState>();
}

void MainMenuState::logOut()
{
    std::cout << "Bye!...\n";
    stackClear();
}


