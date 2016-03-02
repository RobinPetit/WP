// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "client/states/LobbyState.hpp"
#include "client/states/GameState.hpp"
#include "client/NonBlockingInput.hpp"

LobbyState::LobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &LobbyState::quit);
	addAction("Find a game", &LobbyState::startGame);
}

void LobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";

	// Display the actions
	AbstractState::display();
}

void LobbyState::startGame()
{
	std::cout << "Let's find you an opponent\n";
	if(_client.startGame())
		stackPush<GameState>();
	else
		std::cout << "You stopped waiting for an opponent.\n";
}

void LobbyState::quit()
{
	stackClear();
}
