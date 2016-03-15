// std-C++ headers
#include <string>
#include <cassert>
// SFML headers
#include <SFML/Window/Event.hpp>
// WizardPoker headers
#include "client/Gui/GuiAbstractState.hpp"


GuiAbstractState::GuiAbstractState(Context& context):
	AbstractState(context)
{

}

void GuiAbstractState::display()
{
	// The child classes will have to call themselve _context.window->clear, and
	// then call GuiAbstractState::display(). The reason is that the clear has
	// to be done before anything is printed on the window, and the display has
	// to be done after all the printing on the window.
	_context.window->clear(); // TODO move this line in child classes

	_context.gui->draw();
	_context.window->display();
}

void GuiAbstractState::handleInput()
{
	// Process SFML events
	sf::Event event;
	while(_context.window->pollEvent(event))
	{
		// Close window, exit
		if(event.type == sf::Event::Closed)
		{
			_context.window->close();
			stackClear();
		}

		// Give the event to the GUI, to bring menus alive
		_context.gui->handleEvent(event);
	}
}
