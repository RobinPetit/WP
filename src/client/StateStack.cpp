#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

void StateStack::display()
{
	_stack.top()->display();
}

void StateStack::handleEvent()
{
	_stack.top()->handleEvent();
}

void StateStack::pop()
{
	_stack.pop();
}

void StateStack::clear()
{
	_stack = std::stack<std::unique_ptr<AbstractState>>();
}

bool StateStack::isEmpty() const
{
	return _stack.empty();
}
