// WizardPoker headers
#include "client/StateStack.hpp"
#include "client/sockets/Client.hpp"
#include "client/states/AbstractFriendsManagementState.hpp"

AbstractFriendsManagementState::AbstractFriendsManagementState(Context& context):
	AbstractState(context)
{
}

void AbstractFriendsManagementState::backMainMenu()
{
	_context.stateStack->pop();
}

void AbstractFriendsManagementState::treatRequests()
{
	try
	{
		const auto& requests{_context.client->getFriendshipRequests()};
		if(requests.empty())
			displayMessage("You have no incoming request");
		else
			for(const auto& request : requests)
				treatIndividualRequest(request);
	}
	catch(std::runtime_error& e)
	{
		displayMessage(e.what());
	}
}
