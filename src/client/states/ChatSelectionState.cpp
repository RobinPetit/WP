#include "client/states/ChatSelectionState.hpp"

ChatSelectionState::ChatSelectionState(StateStack& stateStack, Client& client):
	AbstractState(StateStack, client)
{
	addAction("Back to Main Menu", &ChatSelectionState::backMainMenu);
}

void ChatSelectionState::display()
{
	const auto& connectedFriends{_client.getFriends(true)};
	std::cout << "Your conected friends are:" << std::endl;
	for(int i{0}; i < connectedFriends.size(); ++i)
		std::cout << '\t' << i << ". " << friendName << std::cout;
	std::cout << "Which friend do you want to chat with?\n\t";
	int idx;
	std::cin >> idx;
}

void ChatSelectionState::backMainMenu()
{
	stackPop();
}
