// WizardPoker headers
#include "client/states/AbstractLobbyState.hpp"

AbstractLobbyState::AbstractLobbyState(Context& context):
	AbstractState(context)
{
}

void AbstractLobbyState::quit()
{
	stackPop();
}
