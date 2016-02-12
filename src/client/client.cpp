/**
	client main file (entry point)
**/

#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Client.hpp>
#include <common/sockets/TransferType.hpp>
#include <iostream>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "must be called as ./WizardPoker_client <name>\n";
		return -1;
	}
	/*std::string name;
	std::cout << "what's your name ?\n\t";
	std::cin >> name;*/
	Client self(argv[1]);
	std::cout << "Welcome " << argv[1] << std::endl;
	if(!self.connectToServer(SERVER_ADDRESS, SERVER_PORT))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	if(std::string(argv[1]) == "player2")
	{
		std::cout << "starting conversation.\n";
		self.startConversation("Poupou");
	}
	int n;
	std::cin >> n;
	std::cout << "quit\n";
	self.quit();
	return CLIENT_OK;
}
