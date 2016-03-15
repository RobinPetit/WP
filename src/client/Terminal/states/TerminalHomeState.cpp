// std-C++ headers
#include <iostream>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "common/UnableToConnectException.hpp"
#include "client/ErrorCode.hpp"
#include "common/ini/IniFile.hpp"
#include "client/Terminal/states/TerminalMainMenuState.hpp"
#include "client/Terminal/states/TerminalHomeState.hpp"

TerminalHomeState::TerminalHomeState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	TerminalAbstractState(stateStack, client),
	AbstractHomeState(stateStack, client)
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

// \TODO: factorize connect and createAccount

void TerminalHomeState::connect()
{
	try
	{
		IniFile config;
		int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
		if(status != SUCCESS)
			throw std::runtime_error("No config file");
		if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
			throw std::runtime_error("Missing data in config file");
		auto identifiers = askIdentifiers();
		sf::Uint16 serverPort{static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE))};
		// The following code is created to handle the case of a crash during
		// server execution and then finding an available port to connect to
		// (associated code in the server)
		bool tryToConnect{true};
		int counter{0};
		while(tryToConnect)
		{
			try
			{
				_client.connectToServer(identifiers.first, identifiers.second,
						config["SERVER_ADDRESS"], serverPort);
				tryToConnect = false;
			}
			catch(const UnableToConnectException& e)
			{
				tryToConnect = (++counter) <= 10;
				++serverPort;
				// Manually break by rethrowing the exception e
				if(not tryToConnect)
					throw;
			}
		}
		stackPush<TerminalMainMenuState>();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Unable to connect to server\n";
		// If the error was caused by client::updateFriends, the client is connected
		// but the user will stay on this state. The client must be deconnected.
		_client.quit();
		waitForEnter();
	}
}

void TerminalHomeState::createAccount()
{
	try
	{
		IniFile config;
		int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
		if(status != SUCCESS)
			throw std::runtime_error("No config file");
		if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
			throw std::runtime_error("Missing data in config file");
		auto identifiers = askIdentifiers();
		_client.registerToServer(identifiers.first,
				identifiers.second,
				config["SERVER_ADDRESS"],
				static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)));
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
