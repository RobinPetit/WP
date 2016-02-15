#include <iostream>
#include "client/states/FriendsManagementState.hpp"

FriendsManagementState::FriendsManagementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &FriendsManagementState::backMainMenu);
	addAction("Add a friend to the list", &FriendsManagementState::addFriend);
	addAction("Remove a friend from the list", &FriendsManagementState::removeFriend);
	// Get the list of friends...
	_friends = {"Emile", "Marc-Henry", "Sigmund"};
}

void FriendsManagementState::display()
{
	std::cout << "Here are your friends:\n";
	for(const auto& friendName : _friends)
		std::cout << "* " << friendName << "\n";

	// Display the actions
	AbstractState::display();
}

void FriendsManagementState::addFriend()
{
	std::cout << "What is the user name of your new friend? ";
	std::string input;
	std::getline(std::cin, input);
	// Get the user data from the user name (input)...
	auto it = std::find(_friends.cbegin(), _friends.cend(), input);
	if(it == _friends.cend())
		_friends.push_back(input);
	else
		std::cout << "It seems that " << input << " is already in your friends list.\n";
}

void FriendsManagementState::removeFriend()
{
	std::cout << "What is the user name of the friend that you want to remove? ";
	std::string input;
	std::getline(std::cin, input);
	auto it = std::find(_friends.cbegin(), _friends.cend(), input);
	if(it != _friends.cend())
		_friends.erase(it);
	else
		std::cout << "It seems that " << input << " is not in your friends list.\n";
}

void FriendsManagementState::backMainMenu()
{
	stackPop();
}


