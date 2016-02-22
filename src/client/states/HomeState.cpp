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

void HomeState::connect()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);

	IniFile config;
	// \TODO: write exceptions for connection errors, then catch them in main
	// and return a status according to the caught exception.
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		throw std::runtime_error("No config file");
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		throw std::runtime_error("Missing data in config file");
	if(!_client.connectToServer(userName, config["SERVER_ADDRESS"], static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE))))
		throw std::runtime_error("Unable to connect to server");

	std::cout << "Hello " << userName << "!\n";
	stackPush<MainMenuState>();
}

void HomeState::createAccount()
{
	std::string userName, password;
	std::cout << "What is your user name? ";
	std::getline(std::cin, userName);
	std::cout << "What is your password? ";
	std::getline(std::cin, password);
	// Do subscribing stuff here
	std::cout << "Hello " << userName << "!\n";
	stackPush<MainMenuState>();
}

void HomeState::quit()
{
	stackClear();
}
