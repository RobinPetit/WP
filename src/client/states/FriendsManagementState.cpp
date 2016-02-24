#include <iostream>
#include "client/states/FriendsManagementState.hpp"

enum
{
	FRIENDSHIP_REQUEST_ACCEPT = 1,
	FRIENDSHIP_REQUEST_REFUSE,
	FRIENDSHIP_REQUEST_IGNORE,
};

FriendsManagementState::FriendsManagementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &FriendsManagementState::backMainMenu);
	addAction("Add a friend to the list", &FriendsManagementState::addFriend);
	addAction("Remove a friend from the list", &FriendsManagementState::removeFriend);
	addAction("Treat friendship requests", &FriendsManagementState::treatRequests);
	addAction("Chat with a friend", &FriendsManagementState::startChat);
}

void FriendsManagementState::display()
{
	const auto& friends(_client.getFriends());
	if(friends.empty())
		std::cout << "You have no friend yet!\n";
	else
	{
		std::cout << "Here is your friends list:\n";
		for(const auto& friendName : friends)
			std::cout << "* " << friendName << "\n";
	}

	// Display the actions
	AbstractState::display();
}

void FriendsManagementState::addFriend()
{
	std::cout << "What is the user name of your new friend? ";
	std::string input;
	std::getline(std::cin, input);
	// Get the user data from the user name (input)...
	if(_client.sendFriendshipRequest(input))
		std::cout << "A friendship request has been sent to " << input << ".\n";
	else
		std::cout << "An error occurred, maybe " << input << " is already in your friends list.\n";
	waitForEnter();
}

void FriendsManagementState::removeFriend()
{
	std::cout << "What is the user name of the friend that you want to remove? ";
	std::string input;
	std::getline(std::cin, input);
	if(_client.removeFriend(input))
		std::cout << input << "has been removed from you friend list.\n";
	else
		std::cout << "An error occurred, " << input << " is not in your friends list.\n";
	waitForEnter();
}

void FriendsManagementState::treatRequests()
{
	const auto& requests(_client.getFriendshipRequests());
	if(requests.size() == 0)
		std::cout << "You have no incoming friendship request.\n";
	else
	{
		for(const auto& name: requests)
		{
			std::cout << name << " wants to become your friend.\n"
			          << FRIENDSHIP_REQUEST_ACCEPT << ". Accept request\n"
			          << FRIENDSHIP_REQUEST_REFUSE << ". Refuse request\n"
			          << FRIENDSHIP_REQUEST_IGNORE << ". Ignore request\n\t";
			int choice;
			std::cin >> choice;
			if(choice == FRIENDSHIP_REQUEST_ACCEPT)
				_client.acceptFriendshipRequest(name);
			else if (choice == FRIENDSHIP_REQUEST_REFUSE)
				_client.acceptFriendshipRequest(name, false);
		}

	}
	waitForEnter();
}

void FriendsManagementState::startChat()
{
	std::string friendName;
	std::cout << "Who do you want to chat with? ";
	std::getline(std::cin, friendName);
	if(!_client.startConversation(friendName))
		std::cout << "Unable to chat with " << friendName << ".\n";
	waitForEnter();
}

void FriendsManagementState::backMainMenu()
{
	stackPop();
}


