// WizardPoker headers
#include "client/states/AbstractFriendsManagementState.hpp"

AbstractFriendsManagementState::AbstractFriendsManagementState(Context& context):
	AbstractState(context)
{
}

void AbstractFriendsManagementState::backMainMenu()
{
	stackPop();
}
