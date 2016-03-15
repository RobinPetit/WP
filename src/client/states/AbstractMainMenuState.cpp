// WizardPoker headers
#include "client/states/AbstractMainMenuState.hpp"

AbstractMainMenuState::AbstractMainMenuState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
}

void AbstractMainMenuState::logOut()
{
	_client.quit();
	stackPop();
}

void AbstractMainMenuState::quit()
{
	stackClear();
}
