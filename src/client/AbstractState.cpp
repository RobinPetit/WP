#include <TheLostGirl/AbstractState.hpp>

void AbstractState::requestStackPop()
{
	_stateStack.popState();
}

void AbstractState::requestStateClear()
{
	_stateStack.clearStates();
}
