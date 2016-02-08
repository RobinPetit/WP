#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

void StateStack::update()
{
	_stack.top()->update();
}

void StateStack::draw()
{
	_stack.top()->draw();
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
