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


HomeState::HomeState(StateStack& stateStack):
    AbstractState(stateStack)
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

	Client self(userName);
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
	if(!self.connectToServer(config["SERVER_ADDRESS"], std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)))
	{
		std::cout << "Unable to connect to server" << std::endl;
		//return UNABLE_TO_CONNECT;
		return;
	}

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
