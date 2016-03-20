// WizardPoker headers
#include "client/StateStack.hpp"
#include "client/states/AbstractMainMenuState.hpp"

AbstractMainMenuState::AbstractMainMenuState(Context& context):
	AbstractState(context)
{
}

void AbstractMainMenuState::logOut()
{
	_context.client->quit();
	_context.stateStack->pop();
}

void AbstractMainMenuState::quit()
{
	_context.stateStack->clear();
}
