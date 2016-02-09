#include "client/AbstractState.hpp"

AbstractState::AbstractState(StateStack& stateStack):
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
