// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalCardsCollectionState.hpp"
#include "client/Terminal/states/TerminalDecksManagementState.hpp"
#include "client/Terminal/states/TerminalFriendsManagementState.hpp"
#include "client/Terminal/states/TerminalLadderState.hpp"
#include "client/Terminal/states/TerminalAchievementState.hpp"
#include "client/Terminal/states/TerminalMainMenuState.hpp"
#include "client/Terminal/states/TerminalLobbyState.hpp"

TerminalMainMenuState::TerminalMainMenuState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractMainMenuState(context)
{
	addAction("Quit", &TerminalMainMenuState::quit);
	addAction("Enter in the matchmaking lobby", &TerminalMainMenuState::findGame);
	addAction("Manage your decks", &TerminalMainMenuState::manageDecks);
	addAction("See your collection of cards", &TerminalMainMenuState::seeCollection);
	addAction("Manage your friends", &TerminalMainMenuState::manageFriends);
	addAction("See the ladder", &TerminalMainMenuState::seeLadder);
	addAction("See your achievements", &TerminalMainMenuState::seeAchievements);
	addAction("Log out", &TerminalMainMenuState::logOut);
}

void TerminalMainMenuState::display()
{
	displaySeparator("Main Menu");

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalMainMenuState::findGame()
{
	_context.stateStack->push<TerminalLobbyState>();
}

void TerminalMainMenuState::manageDecks()
{
	_context.stateStack->push<TerminalDecksManagementState>();
}

void TerminalMainMenuState::seeCollection()
{
	_context.stateStack->push<TerminalCardsCollectionState>();
}

void TerminalMainMenuState::seeLadder()
{
	_context.stateStack->push<TerminalLadderState>();
}

void TerminalMainMenuState::seeAchievements()
{
	_context.stateStack->push<TerminalAchievementState>();
}

void TerminalMainMenuState::manageFriends()
{
	_context.stateStack->push<TerminalFriendsManagementState>();
}
