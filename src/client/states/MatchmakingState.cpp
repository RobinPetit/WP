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
#include "client/states/MatchmakingState.hpp"

MatchmakingState::MatchmakingState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &MatchmakingState::quit);
}

void MatchmakingState::display()
{
	std::cout << "Welcome to the MatchMaking lobby!\n";

	// Display the actions
	AbstractState::display();
}

void MatchmakingState::quit()
{
	stackClear();
}
