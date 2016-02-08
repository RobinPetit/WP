#ifndef _ABSTRACT_STATE_CLIENT_HPP
#define _ABSTRACT_STATE_CLIENT_HPP

#include <memory>
#include <client/StateStack.hpp>

/// Base class for the various states.
/// A state is basically a screen of the application, such as a menu,
/// the title screen, or a game. It encapsulates the logic and the
/// informations of its parts of the application.
/// In order to make a state, you must inherit from this class
/// and implement the pure virtual members.
/// Do not instanciate yourself a state, use StateStack or
/// AbstactState::requestStatePush() instead.
class AbstractState
{
	public:
		/// Constructor.
		/// \param stateStack The current StateStack that holds this state.
		AbstractState(StateStack& stateStack);

		/// Destructor.
		virtual ~AbstractState() = default;

        /// The drawing function.
        /// It must do all things related to drawing stuff on the screen.
		virtual void draw() = 0;

        /// The logic update function.
        /// This function do damages calculations, sending messages to server, etc...
        /// \return Return true if the state under this one in the stack must be also updated.
		virtual bool update() = 0;

        /// The event handling function.
		/// This method get the user input and do things with it.
		virtual void handleEvent() = 0;

	protected:
		/// Add the given state to the pending queue, and construct it as soon as possible.
		/// The construction and deletion of state is delayed because some states can ask
		/// their StateStack to push or pop states while the StateStack is iterating over the states,
		/// resulting in modification of the structure being iterated. By delaying these operations,
		/// we ensure that we modify the stack only when this is possible.
		/// \tparam The type of state to create.
		/// \param args Arguments to forward to the state constructor.
		template<typename T, typename ... Args>
		void requestStackPush(Args&&... args);

		/// Delete the top state as soon as possible.
		void requestStackPop();

		/// Delete all the states as soon as possible.
		void requestStateClear();
	
	private:
		StateStack& _stateStack;
};

template<typename StateType, typename ... Args>
void AbstractState::requestStackPush(Args&&... args)
{
	_stateStack.pushState<StateType>(std::forward<Args>(args)...);
}

#endif// _ABSTRACT_STATE_CLIENT_HPP

