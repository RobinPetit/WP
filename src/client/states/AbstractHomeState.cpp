// WizardPoker headers
#include "common/constants.hpp"
#include "client/sockets/Client.hpp"
#include "common/UnableToConnectException.hpp"
#include "client/ErrorCode.hpp"
#include "common/ini/IniFile.hpp"
#include "client/StateStack.hpp"
#include "client/states/AbstractHomeState.hpp"

AbstractHomeState::AbstractHomeState(Context& context):
	AbstractState(context)
{
}

void AbstractHomeState::quit()
{
	_context.stateStack->clear();
}

void AbstractHomeState::tryToConnect(const std::string& userName, const std::string& password)
{
	auto connectionConfig(getConnectionConfiguration());

#ifdef NDEBUG  // If we aren't in debug
	_context.client->connectToServer(userName, password, connectionConfig.first, connectionConfig.second);
#else  // If we are in debug
	// The following code is created to handle the case of a crash during
	// server execution and then finding an available port to connect to
	// (associated code in the server)
	bool tryToConnect{true};
	int counter{0};
	while(tryToConnect)
	{
		try
		{
			_context.client->connectToServer(userName, password,
									connectionConfig.first, connectionConfig.second);
			tryToConnect = false;
		}
		catch(const UnableToConnectException& e)
		{
			tryToConnect = (++counter) <= 10;
			++connectionConfig.second;
			// Manually break by rethrowing the exception e
			if(not tryToConnect)
				throw;
		}
	}
#endif
}

void AbstractHomeState::tryToRegister(const std::string& userName, const std::string& password)
{
	const auto connectionConfig(getConnectionConfiguration());
	_context.client->registerToServer(userName, password, connectionConfig.first, connectionConfig.second);
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
