/**
	client main file (entry point)
**/

// WizardPoker headers
#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Client.hpp>
#include <common/sockets/TransferType.hpp>
// std-C++ headers
#include <iostream>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "must be called as ./WizardPoker_client <name>\n";
		return -1;
	}
	Client self(argv[1]);
	std::cout << "Welcome " << argv[1] << std::endl;
	if(!self.connectToServer(SERVER_ADDRESS, SERVER_PORT))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	int choice;
	std::cout << "Enter 1 to make a discussion and anything else to quit\n\t";
	std::cin >> choice;
	if(choice == 1)
	{
		std::string name;
		std::cout << "Who do you want to discuss with?\n\t";
		std::cin >> name;
		if(!self.startConversation(name))
			std::cerr << "Error!\n";
		int n;
		std::cout << "Type anything to quit";
		std::cin >> n;
	}
	std::cout << "quit\n";
	self.quit();
	return CLIENT_OK;
}
