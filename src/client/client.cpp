/**
	client main file (entry point)
**/

#include <common/constants.hpp>
#include <client/ErrorCode.hpp>
#include <client/sockets/Connection.hpp>
#include <iostream>

int main()
{
	Connection server("UserName");
	if(!server.connect(SERVER_ADDRESS, SERVER_PORT))
	{
		std::cout << "Unable to connect to server" << std::endl;
		return UNABLE_TO_CONNECT;
	}
	return CLIENT_OK;
}
