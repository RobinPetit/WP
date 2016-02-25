// std-C++ headers
#include <iostream>
// SFML headers
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "client/states/LobbyState.hpp"
#include "client/NonBlockingInput.hpp"

LobbyState::LobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &LobbyState::quit);
}

void LobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";
	NonBlockingInput input;
	sf::SocketSelector selector;
	_client.startGame();

	// Display the actions
	AbstractState::display();
}

void LobbyState::quit()
{
	stackClear();
}
