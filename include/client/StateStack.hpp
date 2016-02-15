#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

#include <stack>
#include <queue>
#include <string>
#include <memory>
#include <functional>
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
/// \see State
class StateStack : private sf::NonCopyable
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

		/// Apply the changes that need to be done.
		/// These changes are stored in _pendingChanges.
		/// When a state calls pop, this state is deleted (its destructor gets
		/// called) but when the pop is done, the stack returns to the caller of
		/// the pop method, which is the state just popped out. Executing code
		/// from a deleted object is undefined behavior, so by delaying the pop
		/// (and the clear) we avoid this undefined behavior.
		void doPendingChanges();

		std::stack<std::unique_ptr<AbstractState>> _stack;///< Stack of state.
		std::queue<std::function<void()>> _pendingChanges;///< Pending changes to do on the stack.
};

template <typename StateType>
void StateStack::push()
{
	_stack.emplace(new StateType(*this));
}

#endif// _STATE_STACK_CLIENT_HPP

