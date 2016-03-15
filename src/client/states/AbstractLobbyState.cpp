// WizardPoker headers
#include "client/states/AbstractLobbyState.hpp"

AbstractLobbyState::AbstractLobbyState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
}

void AbstractLobbyState::quit()
{
	stackPop();
}
