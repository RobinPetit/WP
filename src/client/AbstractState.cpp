// std-C++ headers
#include <iostream>
#include <cassert>
// WizardPoker headers
#include "client/AbstractState.hpp"

AbstractState::AbstractState(Context& context):
	_context{context}
{
}

void AbstractState::onPush()
{
}

void AbstractState::onPop()
{
}
