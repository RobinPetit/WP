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
	_stack.back()->display();
}

void StateStack::handleInput()
{
	_stack.back()->handleInput();
	doPendingChanges();
}

void StateStack::pop()
{
	_pendingChanges.emplace_back([this]()
	{
		delete _stack.back();
		_stack.pop_back();
		if(not _stack.empty())
			_stack.back()->onPop();
	});
}

void StateStack::clear()
{
	_pendingChanges.emplace_back([this]()
	{
		for(auto& ptr : _stack)
			delete ptr;
		_stack.clear();
	});
}

bool StateStack::isEmpty() const
{
	return _stack.empty();
}

void StateStack::doPendingChanges()
{
	for(auto& pendingChange : _pendingChanges)
		pendingChange();
	_pendingChanges.clear();
}
