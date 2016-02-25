// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/CardsCollectionState.hpp"
#include "client/states/DecksManagementState.hpp"
#include "client/states/FriendsManagementState.hpp"
#include "client/states/LadderState.hpp"
#include "client/states/MainMenuState.hpp"
#include "client/states/GameState.hpp"
#include "client/states/LobbyState.hpp"

MainMenuState::MainMenuState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &MainMenuState::quit);
	addAction("Enter in the matchmaking lobby", &MainMenuState::findGame);
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
	stackPush<LobbyState>();
}

void MainMenuState::manageDecks()
{
	stackPush<DecksManagementState>();
}

void MainMenuState::seeCollection()
{
	stackPush<CardsCollectionState>();
}

void MainMenuState::seeLadder()
{
	stackPush<LadderState>();
}

void MainMenuState::manageFriends()
{
	stackPush<FriendsManagementState>();
}

void MainMenuState::logOut()
{
	std::cout << "Bye!...\n";
	_client.quit();
	stackPop();
}

void MainMenuState::quit()
{
	std::cout << "Bye!...\n";
	stackClear();
}


