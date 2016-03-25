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
	std::cout << "Welcome to Wizard Poker!\n";

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalHomeState::connect()
{
	try
	{
		auto connectionConfig(getConnectionConfiguration());
		const auto identifiers(askIdentifiers());
		// The following code is created to handle the case of a crash during
		// server execution and then finding an available port to connect to
		// (associated code in the server)
		bool tryToConnect{true};
		int counter{0};
		while(tryToConnect)
		{
			try
			{
				_context.client->connectToServer(identifiers.first, identifiers.second,
						connectionConfig.first, connectionConfig.second);
				tryToConnect = false;
			}
			catch(const UnableToConnectException& e)
			{
				tryToConnect = (++counter) <= 10;
				++connectionConfig.second;
				// Manually break by rethrowing the exception e
				if(not tryToConnect)
					throw;
			}
		}
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
		const auto connectionConfig(getConnectionConfiguration());
		const auto identifiers(askIdentifiers());
		_context.client->registerToServer(identifiers.first, identifiers.second,
				connectionConfig.first, connectionConfig.second);
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
