#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

StateStack::StateStack(Client& client):
	_stack(),
	_pendingChanges(),
	_client{client}
{

}

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
	_pendingChanges.emplace([this]()
	{
		_stack.pop();
	});
}

void StateStack::clear()
{
	_pendingChanges.emplace([this]()
	{
		_stack = std::stack<std::unique_ptr<AbstractState>>();
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
		// Call the method on top of the queue, then pop it from the queue
		_pendingChanges.front()();
		_pendingChanges.pop();
	}
}
