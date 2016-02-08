#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

#include <vector>
#include <memory>
#include <SFML/System.hpp>

//Forward declarations
class AbstractState;

/// States manager that holds all the states and manages them.
/// \see State
class StateStack : private sf::NonCopyable
{
	public:
		/// Destructor.
		/// This declarations is here in order to avoid a warning about inlining.
		~StateStack() = default;

		/// Call the update() function of all the states in the stack.
		void update();

		/// Call the draw() function of all the states in the stack.
		void draw();

		/// Call the handleEvent() function of the top of the stack.
		void handleEvent();

		/// Add a new state of the templated type on the pending list, and construct it as soon as possible.
		/// The construction and deletion of state is delayed because some states can ask
		/// their StateStack to push or pop states while the StateStack is iterating over the states,
		/// resulting in modification of the structure being iterated. By delaying these operations,
		/// we ensure that we modify the stack only when this is possible.
		/// \tparam StateType The type of the state to construct.
		/// \param args Arguments that will be forwarded to the constructor of the state.
		/// There can be zero, one or more arguments of any types.
		template <typename StateType, typename ... Args>
		void pushState(Args&& ... args);

		/// Delete the top state as soon as possible.
		void popState();

		/// Delete all the states as soon as possible.
		void clearStates();

		/// Check if the stack is empty or not.
		/// \return True if the stack is empty, false otherwise.
		bool isEmpty() const;

	private:
		/// Remove states that need to be removed, constructs others states, ...
		void applyPendingChanges();

		std::vector<std::unique_ptr<AbstractState>> m_stack;///< Array of pointers to the states.
		std::vector<std::function<void()>> m_pendingList;   ///< List of all changes to do in the next update.
};

template <typename StateType, typename ... Args>
void StateStack::pushState(Args&& ... args)
{
	m_pendingList.push_back([this, args...]
	{
		//No perfect forwarding is done here because the arguments
		//needs to be copied in order to be stored in the lambda
		m_stack.push_back(std::unique_ptr<AbstractState>(new StateType(args...)));
	});
}

#endif// _STATE_STACK_CLIENT_HPP

