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
        _pendingChanges.front()();
        _pendingChanges.pop();
    }
}
