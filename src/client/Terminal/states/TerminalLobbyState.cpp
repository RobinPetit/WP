// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/TerminalGame.hpp"
#include "client/Terminal/states/TerminalLobbyState.hpp"

TerminalLobbyState::TerminalLobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	TerminalAbstractState(stateStack, client),
	AbstractLobbyState(stateStack, client)
{
	addAction("Quit", &TerminalLobbyState::quit);
	addAction("Find a game", &TerminalLobbyState::startGame);
}

void TerminalLobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalLobbyState::startGame()
{
	std::cout << "Let's find you an opponent\n";
	if(_client.startGame())
	{
		TerminalGame game{_client};
		game.init();
		game.play();
		waitForEnter();
	}
	else
		std::cout << "You stopped waiting for an opponent.\n";
}
