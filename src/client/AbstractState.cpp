// Standard headers
#include <iostream>
#include <cassert>
// Wizard Poker headers
#include "client/AbstractState.hpp"

AbstractState::AbstractState(StateStack& stateStack, Client& client):
	_client{client},
	_stateStack{stateStack}
{

}

void AbstractState::stackPop()
{
	_stateStack.pop();
}

void AbstractState::stackClear()
{
	_stateStack.clear();
}
