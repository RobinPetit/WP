// WizardPoker headers
#include "client/states/AbstractMainMenuState.hpp"

AbstractMainMenuState::AbstractMainMenuState(Context& context):
	AbstractState(context)
{
}

void AbstractMainMenuState::logOut()
{
	_context.client->quit();
	stackPop();
}

void AbstractMainMenuState::quit()
{
	stackClear();
}
