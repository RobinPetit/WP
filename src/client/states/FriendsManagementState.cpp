#include <iostream>
#include "client/states/FriendsManagementState.hpp"

enum
{
	FRIENDSHIP_REQUEST_ACCEPT,
	FRIENDSHIP_REQUEST_REFUSE,
	FRIENDSHIP_REQUEST_IGNORE,
};

FriendsManagementState::FriendsManagementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Back to main menu", &FriendsManagementState::backMainMenu);
	addAction("Add a friend to the list", &FriendsManagementState::addFriend);
	addAction("Remove a friend from the list", &FriendsManagementState::removeFriend);
	addAction("Check friendship requests", &FriendsManagementState::checkRequests);
	addAction("Treat friendship requests", &FriendsManagementState::treatRequests);
}

void FriendsManagementState::display()
{
	std::cout << "Here are your friends:\n";
	for(const auto& friendName : _client.getFriends())
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
	if(_client.askNewFriend(input))
		std::cout << "A friendship request has been sent to " << input << std::endl;
	else
		std::cout << "It seems that " << input << " is already in your friends list.\n";
}

void FriendsManagementState::removeFriend()
{
	std::cout << "What is the user name of the friend that you want to remove? ";
	std::string input;
	std::getline(std::cin, input);
	if(_client.removeFriend(input))
		std::cout << input << "has been removed from you friend list" << std::endl;
	else
		std::cout << "It seems that " << input << " is not in your friends list.\n";
}

void FriendsManagementState::checkRequests()
{
	std::vector<std::string> acceptedRequests;
	std::vector<std::string> refusedRequests;
	if(!_client.updateFriendshipRequests(acceptedRequests, refusedRequests))
	{
		std::cerr << "Unable to get requests state from the server" << std::endl;
		return;
	}
	if(acceptedRequests.size() != 0)
	{
		std::cout << "\nThe following players accepted your friendship requests:" << std::endl;
		for(const std::string& name: acceptedRequests)
			std::cout << "+ " << name << std::endl;
	}
	if(refusedRequests.size() != 0)
	{
		std::cout << "\nThe following players refused you friendship requests:" << std::endl;
		for(const std::string& name: refusedRequests)
			std::cout << "+ " << name << std::endl;
	}

}

void FriendsManagementState::treatRequests()
{
	std::vector<std::string> incomingRequests;
	if(!_client.getIncomingFriendshipRequests(incomingRequests))
	{
		std::cerr << "Unable to get requests state from the server" << std::endl;
		return;
	}
	if(incomingRequests.size() == 0)
		std::cout << "You have no incoming friendship request" << std::endl;
	else
	{
		for(const std::string& name: incomingRequests)
		{
			std::cout << name << " wants to become your friend.\n"
			          << FRIENDSHIP_REQUEST_ACCEPT << ". Accept request\n"
			          << FRIENDSHIP_REQUEST_REFUSE << ". Refuse request\n"
			          << FRIENDSHIP_REQUEST_IGNORE << ". Ignore request\n\t";
			int choice;
			std::cin >> choice;
			if(choice == FRIENDSHIP_REQUEST_ACCEPT || choice == FRIENDSHIP_REQUEST_REFUSE)
				_client.acceptFriendshipRequest(name, choice == FRIENDSHIP_REQUEST_ACCEPT);
		}

	}
}

void FriendsManagementState::backMainMenu()
{
	stackPop();
}


