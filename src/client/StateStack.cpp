#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

StateStack::StateStack(Client& client):
	_stack(),
	_client{client}
{

}

void StateStack::display()
{
	(*_stackIterator)->display();
}

void StateStack::handleInput()
{
	(*_stackIterator)->handleInput();
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
