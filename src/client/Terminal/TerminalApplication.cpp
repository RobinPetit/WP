// WizardPoker headers
#include "client/Terminal/states/TerminalHomeState.hpp"
#include "client/Terminal/TerminalApplication.hpp"

int TerminalApplication::play()
{
	_stateStack.firstPush<TerminalHomeState>();
	while(not _stateStack.isEmpty())
	{
		system("clear");
		_stateStack.display();
		// Is this try/catch really useful? We do nothing when an exception
		// is caught, the behavior is almost the same as without try/catch.
		try
		{
			_stateStack.handleInput();
		}
		catch(std::runtime_error& error)
		{
			std::cerr << "Exception caught: " << error.what();
			return -1;
		}
	}
	return 0;
}
