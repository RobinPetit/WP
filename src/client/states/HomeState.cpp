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
#include "client/states/HomeState.hpp"

HomeState::HomeState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &HomeState::quit);
	addAction("Connect with your account", &HomeState::connect);
	addAction("Create an account", &HomeState::createAccount);
}

void HomeState::display()
{
	std::cout << "Welcome to Wizard Poker!\n";

	// Display the actions
	AbstractState::display();
}

void HomeState::connect()
{
	auto identifiers = askIdentifiers();
	IniFile config;
	// \TODO: write exceptions for connection errors, then catch them in main
	// and return a status according to the exception catched.
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		//return status;
		return;
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		//return WRONG_FORMAT_CONFIG_FILE;
		return;
	while(!_client.connectToServer(identifiers.first, identifiers.second, config["SERVER_ADDRESS"], std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)))
	{
		std::cout << "Unable to connect to server, try again (or CTRL+C to exit):\n";
		identifiers = askIdentifiers();
	}
	stackPush<MainMenuState>();
}

void HomeState::createAccount()
{
	auto identifiers = askIdentifiers();
	IniFile config;
	// \TODO: write exceptions for connection errors, then catch them in main
	// and return a status according to the exception catched.
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		//return status;
		return;
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		//return WRONG_FORMAT_CONFIG_FILE;
		return;
	while(!_client.registerToServer(identifiers.first, identifiers.second, config["SERVER_ADDRESS"], std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)))
	{
		std::cout << "Unable to register to server, try again (or CTRL+C to exit):" << std::endl;
		identifiers = askIdentifiers();
	}
	if(!_client.connectToServer(identifiers.first, identifiers.second, config["SERVER_ADDRESS"], std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)))
	{
		std::cout << "Unable to connect to the server after registering.\n";
		waitForEnter();
		return;
	}
	stackPush<MainMenuState>();
}

void HomeState::quit()
{
	stackClear();
}

std::pair<std::string, std::string> HomeState::askIdentifiers()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);
	return {userName, password};
}
