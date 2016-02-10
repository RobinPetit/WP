#ifndef _HOME_STATE_CLIENT_HPP
#define _HOME_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class HomeState : public AbstractState
{
    public:
        /// Constructor.
        HomeState(StateStack& stateStack);

        /// The display function.
        /// It must do all things related to drawing or printing stuff on the screen.
        virtual void display() override;

        /// The event handling function.
        virtual void handleInput(const std::string& input) override;

    private:
        void connect();
        void createAccount();

};

#endif// _HOME_STATE_CLIENT_HPP

