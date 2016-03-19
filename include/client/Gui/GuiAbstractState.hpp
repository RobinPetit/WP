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
		/// \note We are using a template for the container rather than
		/// tgui::Container, because we also have to accept tgui::Gui, which is
		/// not a tgui::Container.
		template <class StateType, class ContainerPtr>
		void setupButtons(const std::vector<ButtonData<StateType>>& buttons, ContainerPtr& container);
};


template <class StateType, class ContainerPtr>
void GuiAbstractState::setupButtons(const std::vector<ButtonData<StateType>>& buttons, ContainerPtr& container)
{
	for(auto& button : buttons)
	{
		button.button->setText(button.text);
		button.button->connect("pressed", button.callback, dynamic_cast<StateType*>(this));
		container->add(button.button);
	}
}

#endif  // _GUI_ABSTRACT_STATE_CLIENT_HPP
