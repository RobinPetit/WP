#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

void StateStack::display()
{
    _stack.top()->display();
}

void StateStack::handleInput(const std::string& input)
{
    _stack.top()->handleInput(input);
    doPendingChanges();
}

void StateStack::pop()
{
	_stackIterator--;
}

void StateStack::clear()
{
    _empty = true;
}

bool StateStack::isEmpty() const
{
    return _stack.empty() or _empty;
}

