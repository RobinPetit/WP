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
#include "client/states/GameState.hpp"

GameState::GameState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &GameState::quit);
}

void GameState::display()
{
	std::cout << "Your game is about to start!\n";

	// Display the actions
	AbstractState::display();
}

void GameState::quit()
{
	stackClear();
}
