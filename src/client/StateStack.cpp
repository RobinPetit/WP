#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

StateStack::StateStack(Context& context):
	_stack{},
	_stackIterator{},
	_context{context},
	_empty{false}
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
