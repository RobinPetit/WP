/**
    server entry point
**/

// WizardPoker headers
#include <common/constants.hpp>
#include <server/Server.hpp>
#include <server/ErrorCode.hpp>
#include <common/ini/IniFile.hpp>
#include <common/StrToInt.hpp>

int main()
{
	IniFile config;
	int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		return status;
	Server server;
	if(config.find("SERVER_PORT") == config.end())
		return WRONG_FORMAT_CONFIG_FILE;
	int serverStatus = server.start(strToInt(config["SERVER_PORT"]));
	server.quit();
	return serverStatus;
}
