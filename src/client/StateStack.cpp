#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

StateStack::StateStack(Context& context):
	_stack{},
	_pendingChanges{},
	_context{context}
{

}

void StateStack::display()
{
	_stack.top()->display();
}

void StateStack::handleInput()
{
	_stack.top()->handleInput();
	doPendingChanges();
}

void StateStack::pop()
{
	_pendingChanges.emplace([this]()
	{
		_stack.pop();
		if(not _stack.empty())
			_stack.top()->onPop();
	});
}

void StateStack::clear()
{
	_pendingChanges.emplace([this]()
	{
		decltype(_stack)().swap(_stack);
	});
}

bool StateStack::isEmpty() const
{
	return _stack.empty();
}

void StateStack::doPendingChanges()
{
	while(not _pendingChanges.empty())
	{
		_pendingChanges.front()();
		_pendingChanges.pop();
	}
}
