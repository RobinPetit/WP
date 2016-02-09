#ifndef _FRIENDS_MANAGEMENT_STATE_CLIENT_HPP
#define _FRIENDS_MANAGEMENT_STATE_CLIENT_HPP

#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class FriendsManagementState : public AbstractState
{
    public:
        /// Constructor.
        FriendsManagementState(StateStack& stateStack);

        /// The display function.
        /// It must do all things related to drawing or printing stuff on the screen.
        virtual void display() override;

        /// The event handling function.
        virtual void handleEvent() override;

    private:
        void backMainMenu();

};

#endif// _FRIENDS_MANAGEMENT_STATE_CLIENT_HPP

