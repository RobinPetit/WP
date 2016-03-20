// WizardPoker headers
#include "client/StateStack.hpp"
#include "client/states/AbstractLobbyState.hpp"

AbstractLobbyState::AbstractLobbyState(Context& context):
	AbstractState(context)
{
}

void AbstractLobbyState::quit()
{
	_context.stateStack->pop();
}
