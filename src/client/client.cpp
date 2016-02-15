/**
	client main file (entry point)
**/

// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/Terminal.hpp"
#include "common/ini/IniFile.hpp"
// std-C++ headers
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "must be called as ./WizardPoker_client <name>\n";
		return -1;
	}
	Client self(argv[1]);
	std::cout << "Welcome " << argv[1] << std::endl;
	IniFile config;
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		return status;
	if(config.find("SERVER_PORT") == config.end() || config.find("SERVER_ADDRESS") == config.end())
		return WRONG_FORMAT_CONFIG_FILE;
	if(!self.connectToServer(config["SERVER_ADDRESS"], std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	if(std::string(argv[1]) == "player2")
	{
		//self.addFriend("player1");
		//self.addFriend("Chirac");
	}
	std::cin.ignore();
	std::cout << "showing all of your friends:\n";
	for(const auto& friendName: self.getFriends(false))
		std::cout << friendName << std::endl;
	std::cout << "Are connected:\n";
	for(const auto& friendName: self.getFriends(true))
		std::cout << friendName << std::endl;
	std::cout << "quit\n";
	self.quit();
	return SUCCESS;
}
