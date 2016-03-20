#ifndef _ABSTRACT_STATE_CLIENT_HPP
#define _ABSTRACT_STATE_CLIENT_HPP

// WizardPoker headers
#include "client/Context.hpp"

/// Base class for the various states.
/// A state is basically a screen of the application, such as a menu,
/// the title screen, or a game. It encapsulates the logic and the
/// informations of its parts of the application.
/// In order to make a state, you must inherit from this class
/// and implement the pure virtual members.
/// Do not instanciate yourself a state, use AbstactState::statePush() instead.
///
/// The design of the various state classes works as follow:
/// * We have a base class, AbstractState;
/// * Then we have TerminalAbstractState and GuiAbstractState that inherits
/// from AbstractState;
/// * And we have an abstract class for each state, such as AbstractHomeState;
/// * Each state has also two concrete classes: TerminalHomeState that inherits
/// from TerminalAbstractState and AbstractHomeState, and the corresponding
/// class for the GUI;
///
///
/// TerminalAbstractState does all the work that is not specific to a state, but
/// that is common to all terminal states.
/// GuiAbstractState does all the work that is not specific to a state, but that
/// is common to all GUI states.
///
/// Each abstract state for a menu (such as AbstractHomeState) holds information
/// and do things that is not related to the interface, but that is related
/// to its menu entry.
///
/// This design, although complicated, allows to separate very efficiently the
/// terminal/gui logic, the logic specific to a menu, and the logic for every
/// state.
class AbstractState
{
	public:
		/// Constructor.
		AbstractState(Context& context);

		/// Destructor.
		virtual ~AbstractState() = default;

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() = 0;

		/// The event handling function.
		/// This method get the user input and do things with it.
		virtual void handleInput() = 0;

		/// Method called when another state is pushed on this one.
		/// By default, does nothing.
		virtual void onPush();

		/// Method called when this state become the TOS again (e.g. this method
		/// is called on the HomeState instance when we log out from the main
		/// menu state).
		/// By default, does nothing.
		virtual void onPop();

	protected:
		virtual void displayMessage(const std::string& message) = 0;

		Context& _context;  ///< The context.
};

#endif  // _ABSTRACT_STATE_CLIENT_HPP
