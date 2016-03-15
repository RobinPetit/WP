// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/states/CardsCollectionState.hpp"
#include "client/states/DecksManagementState.hpp"
#include "client/states/FriendsManagementState.hpp"
#include "client/states/LadderState.hpp"
#include "client/states/TerminalMainMenuState.hpp"
#include "client/states/LobbyState.hpp"

TerminalMainMenuState::TerminalMainMenuState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &TerminalMainMenuState::quit);
	addAction("Enter in the matchmaking lobby", &TerminalMainMenuState::findGame);
	addAction("Manage your decks", &TerminalMainMenuState::manageDecks);
	addAction("See your collection of cards", &TerminalMainMenuState::seeCollection);
	addAction("Manage your friends", &TerminalMainMenuState::manageFriends);
	addAction("See the ladder", &TerminalMainMenuState::seeLadder);
	addAction("Log out", &TerminalMainMenuState::logOut);
}

void TerminalMainMenuState::display()
{
	// The menu is not that pretty, it can be improved
	std::cout << std::string(40, '*') << "\n";
	std::cout << "Main menu:\n";

	// Display the actions
	AbstractState::display();
}

void TerminalMainMenuState::findGame()
{
	stackPush<LobbyState>();
}

void TerminalMainMenuState::manageDecks()
{
	stackPush<DecksManagementState>();
}

void TerminalMainMenuState::seeCollection()
{
	stackPush<CardsCollectionState>();
}

void TerminalMainMenuState::seeLadder()
{
	stackPush<LadderState>();
}

void TerminalMainMenuState::manageFriends()
{
	stackPush<FriendsManagementState>();
}

void TerminalMainMenuState::logOut()
{
	std::cout << "Bye!...\n";
	_client.quit();
	stackPop();
}

void TerminalMainMenuState::quit()
{
	std::cout << "Bye!...\n";
	stackClear();
}
