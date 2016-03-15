// std-C++ headers
#include <iostream>
#include <cassert>
// WizardPoker headers
#include "client/AbstractState.hpp"

AbstractState::AbstractState(Context& context):
	_context{context}
{
}

void AbstractState::stackPop()
{
	_context.stateStack->pop();
}

void AbstractState::stackClear()
{
	_context.stateStack->clear();
}
