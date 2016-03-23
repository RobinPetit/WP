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
#include "client/Gui/states/ButtonData.hpp"

/// GUI specialisation of AbstractState.
/// Does all the work that is not specific to a state, but that is common to all
/// GUI states.
class GuiAbstractState : virtual public AbstractState
{
	public:
		/// Constructor.
		/// \param context The context of the application.
		GuiAbstractState(Context& context);

		/// Destructor. Deletes all root widgets of this state.
		virtual ~GuiAbstractState() override;

		/// Method called when another state is pushed on this one.
		/// Hides all root widgets.
		virtual void onPush() override;

		/// Method called when this state become the TOS again (e.g. this method
		/// is called on the HomeState instance when we log out from the main
		/// menu state).
		/// Shows all root widgets again.
		virtual void onPop() override;

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

		/// Creates the buttons from a vector of ButtonsData, and add them to
		/// a container.
		/// \param buttonsData The vector of buttons data to create.
		/// \param container A pointer to container to add the buttons to.
		template <class StateType>
		void setupButtons(const std::vector<ButtonData<StateType>>& buttons, tgui::Container::Ptr container);

		/// Tell which widgets are the main widgets, the ones that need to be
		/// hidden/shown/removed when the state stack is modified. This may be
		/// the main layout, the title label, and so on. The overload of onPush,
		/// onPop and of the destructor will then manage these widgets
		/// accordingly.
		/// \param widgets The root widgets.
		void registerRootWidgets(std::vector<tgui::Widget::Ptr>&& widgets);

	private:
		/// See \ref registerRootWidget for explanation of this attribute.
		std::vector<tgui::Widget::Ptr> _rootWidgets;
};


template <class StateType>
void GuiAbstractState::setupButtons(const std::vector<ButtonData<StateType>>& buttons, tgui::Container::Ptr container)
{
	for(auto& button : buttons)
	{
		button.button->setText(button.text);
		button.button->connect("pressed", button.callback, dynamic_cast<StateType*>(this));
		container->add(button.button);
	}
}

#endif  // _GUI_ABSTRACT_STATE_CLIENT_HPP
