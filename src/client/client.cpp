/**
	client main file (entry point)
**/

#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Connection.hpp>
#include <common/sockets/TransferType.hpp>
#include <iostream>

int main()
{
	std::string name;
	std::cout << "what's your name ?\n\t";
	std::cin >> name;
	Connection server(name);
	if(!server.connect(SERVER_ADDRESS, SERVER_PORT))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	return CLIENT_OK;
}
