// WizardPoker headers
#include "client/states/AbstractFriendsManagementState.hpp"

AbstractFriendsManagementState::AbstractFriendsManagementState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
}

void AbstractFriendsManagementState::backMainMenu()
{
	stackPop();
}
