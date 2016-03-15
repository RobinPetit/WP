// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "common/UnableToConnectException.hpp"
//#include "client/Terminal/states/GuiMainMenuState.hpp"
#include "client/Gui/states/GuiHomeState.hpp"

GuiHomeState::GuiHomeState(Context& context):
	AbstractState(context),
	GuiAbstractState(context),
	AbstractHomeState(context)
{
}

void GuiHomeState::connect()
{
	try
	{
		auto connectionConfig(getConnectionConfiguration());
		// TODO use GUI to instanciate this
		std::pair<std::string, std::string> identifiers{"testing", "b"};
		// The following code is created to handle the case of a crash during
		// server execution and then finding an available port to connect to
		// (associated code in the server)
		bool tryToConnect{true};
		int counter{0};
		while(tryToConnect)
		{
			try
			{
				_context.client->connectToServer(identifiers.first, identifiers.second,
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
		//stackPush<GuiMainMenuState>();
	}
	catch(const std::runtime_error& e)
	{
		// TODO display error message about the exception
		// If the error was caused by client::updateFriends, the client is connected
		// but the user will stay on this state. The client must be deconnected.
		_context.client->quit();
	}
}

void GuiHomeState::createAccount()
{
	try
	{
		const auto connectionConfig(getConnectionConfiguration());
		// TODO use GUI to instanciate this
		std::pair<std::string, std::string> identifiers = {"newaccount", "password"};
		_context.client->registerToServer(identifiers.first, identifiers.second,
		                         connectionConfig.first, connectionConfig.second);
	}
	catch(const std::runtime_error& e)
	{
		// TODO display error message about the exception
	}
}
