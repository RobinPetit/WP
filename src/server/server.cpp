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
	int serverStatus = server.start(static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE)));
	return serverStatus;
}
