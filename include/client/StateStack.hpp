#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

#include <stack>
#include <queue>
#include <string>
#include <memory>
#include <functional>
#include <typeinfo>
#include <SFML/System.hpp>

//Forward declarations
class AbstractState;

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
		/// Destructor.
		/// This declarations is here in order to avoid a warning about inlining.
		~StateStack() = default;

		/// Call the display() function of the top state.
		void display();

		/// Call the handleInput() function of the top state.
		void handleInput(const std::string& input);

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
		bool _empty = false;  ///< Indicates wether clear() gets called.
};

template <typename StateType>
void StateStack::push()
{
	// If we do the first push or if the iterator is at TOS
	if(_stack.empty() or *_stackIterator == _stack.back())
	{
		_stack.emplace_back(new StateType(*this));
		_stackIterator = _stack.end() - 1;
	}
	// If we must store another state type at *(_stackIterator + 1)
	else if(typeid(StateType*) != typeid((_stackIterator + 1)->get()))
	{
		_stackIterator++;
		_stackIterator->reset(new StateType(*this));
	}
}

#endif// _STATE_STACK_CLIENT_HPP

