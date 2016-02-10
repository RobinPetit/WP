#ifndef _ABSTRACT_STATE_CLIENT_HPP
#define _ABSTRACT_STATE_CLIENT_HPP

#include <memory>
#include <vector>
#include <utility>// std::pair
#include <string>
#include <functional>// std::function and std::bind
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
        /// \param input The characters that the user typed (an entire line).
        virtual void handleInput(const std::string& input) = 0;

    protected:
        /// Add a new state to the stack.
        /// \tparam The type of state to create.
        template<typename StateType>
        void stackPush();

        /// Delete the top state.
        void stackPop();

        /// Delete all the states.
        void stackClear();

        /// Helper method for adding an entry in the menu.
        /// \param actionName The name of the action.
        /// \param method A pointer to member that will be called when the
        /// action is triggered.
        template <class StateType>
        void addAction(const std::string& actionName, void (StateType::*method)());

        std::vector<std::pair<std::string, std::function<void()>>> _actions;///< All actions doable in the state.

    private:
        StateStack& _stateStack;
};

template<typename StateType>
void AbstractState::addAction(const std::string& actionName, void (StateType::*method)())
{
    // emplace_back simply add an element to the end of the vector.
    // std::bind constructs a function that results in the call
    // of method with *this as implicit first argument.
    _actions.emplace_back(actionName, std::bind(method, dynamic_cast<StateType&>(*this)));
}

template<typename StateType>
void AbstractState::stackPush()
{
    _stateStack.push<StateType>();
}

#endif// _ABSTRACT_STATE_CLIENT_HPP

