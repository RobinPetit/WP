/**
	client main file (entry point)
**/

// std-C++ headers
#include <iostream>
#include <cstdlib>
// WizardPoker heaaders
#include "client/StateStack.hpp"
#include "client/Terminal/states/TerminalHomeState.hpp"
#include "client/sockets/Client.hpp"

int main(int argc, char* argv[])
{
	bool noGui{true};
	// TODO: once the GUI is implemented, the ligne above should be:
	// bool noGui{false};
	if(argc > 1 and std::string(argv[1]) == "--no-gui")
		noGui = true;
	Client client;
	StateStack stack(client);
	if(noGui)
		stack.push<TerminalHomeState>();
	else
	{
		//stack.push<GuiHomeState>();
	}

	while(not stack.isEmpty())
	{
		if(noGui)
			system("clear");
		stack.display();
		// Is this try/catch really useful? We do nothing when an exception
		// is caught, the behavior is almost the same as without try/catch.
		try
		{
			stack.handleInput();
		}
		catch(std::runtime_error& error)
		{
			std::cerr << "Exception caught: " << error.what();
			return -1;
		}
	}
	return 0;
}
