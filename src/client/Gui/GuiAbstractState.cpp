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
	_context.window->clear();
	_context.window->display();
}

void GuiAbstractState::handleInput()
{
	// Process SFML events
	sf::Event event;
	while(_context.window->pollEvent(event))
	{
		// Close window: exit
		if(event.type == sf::Event::Closed)
		{
			_context.window->close();
			stackClear();
		}
	}
}
