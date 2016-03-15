// WizardPoker headers
#include "client/states/AbstractHomeState.hpp"

AbstractHomeState::AbstractHomeState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
}

void AbstractHomeState::quit()
{
	stackClear();
}
