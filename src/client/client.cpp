/**
	client main file (entry point)
**/

// std-C++ headers
#include <iostream>
// WizardPoker heaaders
#include "client/StateStack.hpp"
#include "client/states/HomeState.hpp"
#include "client/sockets/Client.hpp"

int main()
{
	Client client;
	StateStack stack(client);
	stack.push<HomeState>();
	while(not stack.isEmpty())
	{
		system("clear");
		stack.display();
		std::cout << "What do you want to do? ";
		std::string input;
		std::getline(std::cin, input);
		stack.handleInput(input);
	}
	return 0;
}
