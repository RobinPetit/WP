// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "client/states/AbstractLobbyState.hpp"
#include "client/NonBlockingInput.hpp"
#include "client/Terminal/TerminalGame.hpp"

AbstractLobbyState::AbstractLobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &AbstractLobbyState::quit);
	addAction("Find a game", &AbstractLobbyState::startGame);
}

void AbstractLobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";

	// Display the actions
	AbstractState::display();
}

void AbstractLobbyState::startGame()
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

void AbstractLobbyState::quit()
{
	stackPop();
}
