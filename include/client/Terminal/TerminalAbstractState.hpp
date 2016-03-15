#ifndef _TERMINAL_ABSTRACT_STATE_CLIENT_HPP
#define _TERMINAL_ABSTRACT_STATE_CLIENT_HPP

// std-C++ headers
#include <vector>
#include <utility>  // std::pair
#include <functional>  // std::function and std::bind
// WizardPoker headers
#include "client/StateStack.hpp"
#include "client/AbstractState.hpp"

/// Terminal specialisation of AbstractState.
/// Does all the work that is not specific to a state, but that is common to all
/// terminal states.
class TerminalAbstractState : virtual public AbstractState
{
	public:
		/// Constructor.
		TerminalAbstractState(Context& context);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the
		/// screen.
		virtual void display() override;

		/// The event handling function.
		/// This method get the user input and do things with it.
		virtual void handleInput() override;

		/// Asks the user for a number.
		/// \param from The minimum value of the number.
		/// \param to The maximum value of the number, +1.
		/// \return A number in [from, to[.
		/// \throw std::out_of_range if the user gives a wrong number.
		static std::size_t askForNumber(std::size_t from, std::size_t to);

	protected:
		/// Helper method for adding an entry in the menu.
		/// The first call to this function should be for adding the 'quit'
		/// entry or similar.
		/// \param actionName The name of the action.
		/// \param method A pointer to member that will be called when the
		/// action is triggered.
		template <class StateType>
		void addAction(const std::string& actionName, void (StateType::*method)());

		/// Ask the user to press enter to continue. This is useful to display
		/// something just before the call to clear, so that the user has not to
		/// scroll up in order to see what's just displayed.
		static void waitForEnter();

	private:
		/// All actions doable in the state.
		std::vector<std::pair<std::string, std::function<void()>>> _actions;
};

template<typename StateType>
void TerminalAbstractState::addAction(const std::string& actionName, void (StateType::*method)())
{
	// emplace_back simply add an element to the end of the vector.
	// std::bind constructs a function that results in the call
	// of method with this as implicit first argument.
	_actions.emplace_back(actionName, std::bind(method, dynamic_cast<StateType*>(this)));
}

#endif  // _TERMINAL_ABSTRACT_STATE_CLIENT_HPP
