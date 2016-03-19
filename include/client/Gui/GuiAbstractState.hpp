#ifndef _GUI_ABSTRACT_STATE_CLIENT_HPP
#define _GUI_ABSTRACT_STATE_CLIENT_HPP

// std-C++ headers
#include <vector>
#include <utility>  // std::pair
#include <functional>  // std::function and std::bind
// SFML headers
#include <SFML/Graphics/RenderWindow.hpp>
// WizardPoker headers
#include "client/StateStack.hpp"
#include "client/AbstractState.hpp"

/// GUI specialisation of AbstractState.
/// Does all the work that is not specific to a state, but that is common to all
/// GUI states.
class GuiAbstractState : virtual public AbstractState
{
	public:
		/// Constructor.
		/// \param context The context of the application.
		GuiAbstractState(Context& context);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the
		/// screen.
		virtual void display() override;

		/// The event handling function.
		/// This method get the user input and do things with it.
		virtual void handleInput() override;

	protected:
		/// Display a GUI message box with \a message as content, and with
		/// a button Ok.
		/// \param message The message to display in the message box.
		void displayMessage(const std::string& message) override;
};

#endif  // _GUI_ABSTRACT_STATE_CLIENT_HPP
