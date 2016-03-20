#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

// std-C++ headers
#include <stack>
#include <string>
#include <memory>
#include <functional>
#include <typeinfo>
// SFML headers
#include <SFML/System.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
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

		/// Delete the top state.
		void pop();

		/// Delete all the states.
		void clear();

		/// Check if the stack is empty or not.
		/// \return True if the stack is empty, false otherwise.
		bool isEmpty() const;

	private:
		std::vector<std::unique_ptr<AbstractState>> _stack;  ///< Vector of state.
		std::vector<std::unique_ptr<AbstractState>>::iterator _stackIterator;  ///< Iterator on _stack.
		Context& _context;
		bool _empty;  ///< Indicates wether clear() gets called.
};

template <typename StateType>
void StateStack::push()
{
	// If we do the first push or if the iterator is at TOS
	if(_stack.empty() or *_stackIterator == _stack.back())
	{
		if(not _stack.empty())
			(*_stackIterator)->onPush();
		_stack.emplace_back(new StateType(_context));
		_stackIterator = _stack.end() - 1;
	}
	else
	{
		(*_stackIterator)->onPush();
		_stackIterator++;
		_stackIterator->reset(new StateType(_context));
	}
}

#endif// _STATE_STACK_CLIENT_HPP
