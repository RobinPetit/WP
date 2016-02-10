/**
	client main file (entry point)
**/

#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Client.hpp>
#include <common/sockets/TransferType.hpp>
#include <iostream>

int main()
{
	std::string name;
	std::cout << "what's your name ?\n\t";
	std::cin >> name;
	Client self(name);
	std::cout << "Welcome " << name << std::endl;
	if(!self.connectToServer(SERVER_ADDRESS, SERVER_PORT))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	if(name == "player2")
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
