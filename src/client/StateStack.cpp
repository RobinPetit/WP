#include <TheLostGirl/AbstractState.hpp>
#include <TheLostGirl/StateStack.hpp>

void StateStack::update()
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for(auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
		if(not (*itr)->update(dt))
			break;
	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for(auto& state : m_stack)
		state->draw();
}

void StateStack::handleEvent()
{
	m_stack.back().handleEvent();
	applyPendingChanges();
}

void StateStack::popState()
{
	m_pendingList.push_back(std::bind(&std::vector<std::unique_ptr<AbstractState>>::pop_back, &m_stack));
}

void StateStack::clearStates()
{
	m_pendingList.emplace_back(std::bind(&std::vector<std::unique_ptr<AbstractState>>::clear, &m_stack));
}

bool StateStack::isEmpty() const
{
	return m_stack.empty();
}

void StateStack::applyPendingChanges()
{
	for(auto& change : m_pendingList)
		change();
	m_pendingList.clear();
}

