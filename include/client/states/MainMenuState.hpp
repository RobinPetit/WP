#ifndef _MAIN_MENU_STATE_CLIENT_HPP
#define _MAIN_MENU_STATE_CLIENT_HPP

#include <memory>
#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class MainMenuState : public AbstractState
{
	public:
        /// Constructor.
		MainMenuState(StateStack& stateStack);

        /// The drawing function.
        /// \return virtual void
        /// It must do all things related to printing stuff on the screen.
		virtual void draw() override;

        /// The logic update function.
        /// \return Return true if the state under this one in the stack must be also updated.
		virtual void update() override;

        /// The event handling function.
		virtual void handleEvent() override;
};

#endif// _MAIN_MENU_STATE_CLIENT_HPP

