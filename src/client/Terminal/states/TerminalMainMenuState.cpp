// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalCardsCollectionState.hpp"
#include "client/Terminal/states/TerminalDecksManagementState.hpp"
#include "client/Terminal/states/TerminalFriendsManagementState.hpp"
#include "client/Terminal/states/TerminalLadderState.hpp"
#include "client/Terminal/states/TerminalMainMenuState.hpp"
#include "client/Terminal/states/TerminalLobbyState.hpp"

TerminalMainMenuState::TerminalMainMenuState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	TerminalAbstractState(stateStack, client),
	AbstractMainMenuState(stateStack, client)
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
	TerminalAbstractState::display();
}

void TerminalMainMenuState::findGame()
{
	stackPush<TerminalLobbyState>();
}

void TerminalMainMenuState::manageDecks()
{
	stackPush<TerminalDecksManagementState>();
}

void TerminalMainMenuState::seeCollection()
{
	stackPush<TerminalCardsCollectionState>();
}

void TerminalMainMenuState::seeLadder()
{
	stackPush<TerminalLadderState>();
}

void TerminalMainMenuState::manageFriends()
{
	stackPush<TerminalFriendsManagementState>();
}
