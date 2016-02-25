// std-C++ headers
#include <iostream>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "client/sockets/Client.hpp"
#include "common/ini/IniFile.hpp"
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

// \TODO: factorize connect and createAccount

void HomeState::connect()
{
	auto identifiers = askIdentifiers();
	IniFile config;
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		throw std::runtime_error("No config file");
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		throw std::runtime_error("Missing data in config file");
	while(!_client.connectToServer(identifiers.first, identifiers.second, config["SERVER_ADDRESS"], static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE))))
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
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		throw std::runtime_error("No config file");
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		throw std::runtime_error("Missing data in config file");
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
