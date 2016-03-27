// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "common/UnableToConnectException.hpp"
#include "client/Terminal/states/TerminalMainMenuState.hpp"
#include "client/Terminal/states/TerminalHomeState.hpp"

TerminalHomeState::TerminalHomeState(Context& context):
	AbstractState(context),
	TerminalAbstractState(context),
	AbstractHomeState(context)
{
	addAction("Quit", &TerminalHomeState::quit);
	addAction("Connect with your account", &TerminalHomeState::connect);
	addAction("Create an account", &TerminalHomeState::createAccount);
}

void TerminalHomeState::display()
{
	displaySeparator("Welcome to Wizard Poker");

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalHomeState::connect()
{
	try
	{
		const auto identifiers(askIdentifiers());
		tryToConnect(identifiers.first, identifiers.second);
		_context.stateStack->push<TerminalMainMenuState>();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Unable to connect to server\n";
		// If the error was caused by client::updateFriends, the client is connected
		// but the user will stay on this state. The client must be deconnected.
		_context.client->quit();
		waitForEnter();
	}
}

void TerminalHomeState::createAccount()
{
	try
	{
		const auto identifiers(askIdentifiers());
		tryToRegister(identifiers.first, identifiers.second);
		std::cout << "You have been successfuly registered!\n";
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Unable to register to server, try again.\n";
	}
	waitForEnter();
}

std::pair<std::string, std::string> TerminalHomeState::askIdentifiers()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);
	return {userName, password};
}
