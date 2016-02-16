/**
	client main file (entry point)
**/

// std-C++ headers
#include <iostream>
#include <cstdlib>
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


	/*
	if(std::string(argv[1]) == "player2")
	{
		//self.addFriend("player1");
		//self.addFriend("Chirac");
	}
	std::cin.ignore();
	std::cout << "showing all of your friends:\n";
	for(const auto& friendName: self.getFriends(false))
		std::cout << friendName << std::endl;
	std::cout << "Are connected:\n";
	for(const auto& friendName: self.getFriends(true))
		std::cout << friendName << std::endl;
	std::cout << "quit\n";
	self.quit();*/

	return 0;
}
