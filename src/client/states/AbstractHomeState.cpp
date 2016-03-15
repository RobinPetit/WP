// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "common/ini/IniFile.hpp"
#include "client/states/AbstractHomeState.hpp"

AbstractHomeState::AbstractHomeState(Context& context):
	AbstractState(context)
{
}

void AbstractHomeState::quit()
{
	stackClear();
}

std::pair<std::string, sf::Uint16> AbstractHomeState::getConnectionConfiguration()
{
	IniFile config;
	const int status = config.readFromFile(SERVER_CONFIG_FILE_PATH);
	if(status != SUCCESS)
		throw std::runtime_error("No config file");
	if(config.find("SERVER_ADDRESS") == config.end() || config.find("SERVER_PORT") == config.end())
		throw std::runtime_error("Missing data in config file");

	const std::string serverAddress{config["SERVER_ADDRESS"]};
	const sf::Uint16 serverPort{static_cast<sf::Uint16>(std::stoi(config["SERVER_PORT"], nullptr, AUTO_BASE))};
	return {serverAddress, serverPort};
}
