/**
	server entry point
**/

// WizardPoker headers
#include "common/constants.hpp"
#include "server/Server.hpp"
#include "server/ErrorCode.hpp"
#include "common/ini/IniFile.hpp"
// std-C++ headers
#include <cstdlib>

int main()
{
	IniFile config;
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		return status;
	Server server;
	if(config.find("SERVER_PORT") == config.end())
		return WRONG_FORMAT_CONFIG_FILE;
	sf::Uint16 serverPort{static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE))};
	int serverStatus;
	// Same as client: loop 10 times to find an available port
	int counter{0};
	while(((serverStatus = server.start(serverPort++)) == UNABLE_TO_LISTEN) && ((++counter) > 10))
		;
	return serverStatus;
}
