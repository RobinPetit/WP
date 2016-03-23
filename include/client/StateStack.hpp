#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

// std-C++ headers
#include <stack>
#include <queue>
#include <memory>
#include <functional>
// WizardPoker headers
#include "client/Context.hpp"
#include "client/AbstractState.hpp"

//Forward declarations
class Context;

/// States manager that holds all the states and manages them.
/// Structuring states as a stack allows more natural browsing in the menus
/// without overhead in the code.
/// For example, an option menu can be pushed in top of the main menu, or in top
/// of the game state, and when it will be poped, the stack will naturally hand
/// over to the previous state (no matter if it was the main menu or the game
/// state).
/// TODO: ajouter explication push & pop
/// \see AbstractState
class StateStack
{
	public:
		/// Constructor
		StateStack(Context& context);

		/// Destructor.
		/// This declarations is here in order to avoid a warning about inlining.
		~StateStack() = default;

		/// Calls the display() function of the top state.
		void display();

		/// Calls the handleInput() function of the top state.
		void handleInput();

		/// Add a new state of the template type to the top of the stack.
		/// \tparam StateType The type of the state to construct.
		template <typename StateType>
		void push();

		template <typename StateType>
		void firstPush();

		/// Delete the top state.
		void pop();

		/// Delete all the states.
		void clear();

		/// Check if the stack is empty or not.
		/// \return True if the stack is empty, false otherwise.
		bool isEmpty() const;

	private:
		std::stack<std::unique_ptr<AbstractState>> _stack;  ///< Vector of state.
		std::queue<std::function<void()>> _pendingChanges;
		Context& _context;

		void doPendingChanges();
};

template <typename StateType>
void StateStack::push()
{
	_pendingChanges.emplace([this]()
	{
		if(not _stack.empty())
			_stack.top()->onPush();
		_stack.emplace(new StateType(_context));
	});
}

template <typename StateType>
void StateStack::firstPush()
{
	_stack.emplace(new StateType(_context));
}

#endif  // _STATE_STACK_CLIENT_HPP
