// std-C++ headers
#include <iostream>
#include <cstdlib>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Client.hpp>
#include <common/sockets/TransferType.hpp>
#include <common/Terminal.hpp>
#include <common/ini/IniFile.hpp>
#include "client/states/MainMenuState.hpp"
#include "client/states/LobbyState.hpp"

LobbyState::LobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &LobbyState::quit);
}

void LobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";

	// Display the actions
	AbstractState::display();
}

void LobbyState::quit()
{
	stackClear();
}
