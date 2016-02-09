#ifndef _ABSTRACT_STATE_CLIENT_HPP
#define _ABSTRACT_STATE_CLIENT_HPP

#include <memory>
#include "client/StateStack.hpp"

/// Base class for the various states.
/// A state is basically a screen of the application, such as a menu,
/// the title screen, or a game. It encapsulates the logic and the
/// informations of its parts of the application.
/// In order to make a state, you must inherit from this class
/// and implement the pure virtual members.
/// Do not instanciate yourself a state, use AbstactState::statePush() instead.
class AbstractState
{
    public:
        /// Constructor.
        /// \param stateStack The stack that manages this state.
        AbstractState(StateStack& stateStack);

        /// Destructor.
        virtual ~AbstractState() = default;

        /// The display function.
        /// It must do all things related to drawing or printing stuff on the screen.
        virtual void display() = 0;

        /// The event handling function.
        /// This method get the user input and do things with it.
        virtual void handleEvent() = 0;

    protected:
        /// Add a new state to the stack.
        /// \tparam The type of state to create.
        template<typename StateType>
        void stackPush();

        /// Delete the top state.
        void stackPop();

        /// Delete all the states.
        void stackClear();

    private:
        StateStack& _stateStack;
};

template<typename StateType>
void AbstractState::stackPush()
{
    _stateStack.push<StateType>();
}

#endif// _ABSTRACT_STATE_CLIENT_HPP

