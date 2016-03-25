// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/Terminal/TerminalGame.hpp"
#include "client/Terminal/states/TerminalLobbyState.hpp"

TerminalLobbyState::TerminalLobbyState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractLobbyState(context)
{
	addAction("Quit", &TerminalLobbyState::quit);
	addAction("Find a game", &TerminalLobbyState::findAGame);
}

void TerminalLobbyState::display()
{
	std::cout << "Welcome to the matchmaking lobby!\n";

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalLobbyState::findAGame()
{
	if(not _context.client->isConnected())
	{
		std::cout << "You can't play: you're not connected!\n";
		waitForEnter();
		return;
	}

	static const std::string cancelWaitingString{"q"};
	std::cout << "Let's find you an opponent. Type " << cancelWaitingString << " to leave the lobby\n";
	_context.client->enterLobby();
	NonBlockingInput input;
	std::string opponentName;
	while(true)
	{
		if(_context.client->isGameStarted(opponentName))
			break;
		else if(input.waitForData(0.05) && input.receiveStdinData() == cancelWaitingString)
		{
			_context.client->leaveLobby();
			std::cout << "You decided to leave the waiting lobby" << std::endl;
			return;
		}
	}
        std::cout << "Opponent found: " << opponentName << std::endl;
	TerminalGame game{*_context.client};
	startGame(game);
	waitForEnter();
}
