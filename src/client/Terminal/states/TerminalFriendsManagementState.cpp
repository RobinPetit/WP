// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/Terminal/states/TerminalFriendsManagementState.hpp"

enum
{
	FRIENDSHIP_REQUEST_ACCEPT = 1,
	FRIENDSHIP_REQUEST_REFUSE,
	FRIENDSHIP_REQUEST_IGNORE,
};

TerminalFriendsManagementState::TerminalFriendsManagementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	TerminalAbstractState(stateStack, client),
	AbstractFriendsManagementState(stateStack, client)
{
	addAction("Back to main menu", &TerminalFriendsManagementState::backMainMenu);
	addAction("Add a friend to the list", &TerminalFriendsManagementState::addFriend);
	addAction("Remove a friend from the list", &TerminalFriendsManagementState::removeFriend);
	addAction("Treat friendship requests", &TerminalFriendsManagementState::treatRequests);
	addAction("Chat with a friend", &TerminalFriendsManagementState::startChat);
}

void TerminalFriendsManagementState::display()
{
	try
	{
		const auto& friends(_client.getFriends());
		if(friends.empty())
			std::cout << "You have no friend yet!\n";
		else
		{
			std::cout << "Here is your friends list:\n";
			for(const auto& friendUser : friends)
				std::cout << "* " << friendUser.name << "\n";
		}
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	// Display the actions
	TerminalAbstractState::display();
}

void TerminalFriendsManagementState::addFriend()
{
	std::cout << "What is the user name of your new friend? ";
	std::string input;
	std::getline(std::cin, input);
	// Get the user data from the user name (input)...
	try
	{
		_client.sendFriendshipRequest(input);
		std::cout << "A friendship request has been sent to " << input << ".\n";
	}
	catch(std::runtime_error& error)
	{
		std::cerr << "Error: " << error.what() << "\n";
	}
	waitForEnter();
}

void TerminalFriendsManagementState::removeFriend()
{
	std::cout << "What is the user name of the friend that you want to remove? ";
	std::string input;
	std::getline(std::cin, input);
	try
	{
		_client.removeFriend(input);
		std::cout << input << "has been removed from you friend list.\n";
	}
	catch(std::runtime_error& error)
	{
		std::cerr << "Error: " << error.what() << "\n";
	}
	waitForEnter();
}

void TerminalFriendsManagementState::treatRequests()
{
	try
	{
		const auto& requests(_client.getFriendshipRequests());
		if(requests.empty())
			std::cout << "You have no incoming friendship request.\n";
		else
		{
			for(const auto& user: requests)
			{
				std::cout << user.name << " wants to become your friend.\n"
						  << FRIENDSHIP_REQUEST_ACCEPT << ". Accept request\n"
						  << FRIENDSHIP_REQUEST_REFUSE << ". Refuse request\n"
						  << FRIENDSHIP_REQUEST_IGNORE << ". Ignore request\n\t";
				int choice;
				std::cin >> choice;
				if(choice == FRIENDSHIP_REQUEST_ACCEPT)
					_client.acceptFriendshipRequest(user.name);
				else if (choice == FRIENDSHIP_REQUEST_REFUSE)
					_client.acceptFriendshipRequest(user.name, false);
			}

		}
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
	waitForEnter();
}

void TerminalFriendsManagementState::startChat()
{
	std::string friendName;
	std::cout << "Who do you want to chat with? ";
	std::getline(std::cin, friendName);
	try
	{
		_client.startConversation(friendName);
	}
	catch(const std::runtime_error& error)
	{
		std::cerr << "Unable to chat with " << friendName << ": " << error.what() << "\n";
	}
	waitForEnter();
}
