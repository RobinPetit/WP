/**
    server entry point
**/

// WizardPoker headers
#include <common/constants.hpp>
#include <common/ConfigParser.hpp>
#include <server/Server.hpp>
#include <server/ErrorCode.hpp>

int main()
{
	ConfigData data;
	int status = ConfigParser::readFromFile(SERVER_CONFIG_FILE_PATH, data);
	if(status != SUCCESS)
		return status;
	return Server().start(data.port);
}
