// WizardPoker headers
#include "client/Gui/states/GuiHomeState.hpp"
#include "client/Gui/GuiApplication.hpp"

GuiApplication::GuiApplication():
	_videoMode{800, 600},
	_windowTitle{"Wizard Poker"},
	_window{_videoMode, _windowTitle},
	_gui{_window}
{
	_context.window = &_window;
	_context.gui = &_gui;

	// Fix a bug that occurs on Theo's computer, showing a "This window does not
	// respond, do you wanna kill it?" prompt after a few seconds when running
	// the application
	sf::Event event;
	while(_window.pollEvent(event));
}

int GuiApplication::play()
{
	_stateStack.push<GuiHomeState>();
	while(not _stateStack.isEmpty())
	{
		_stateStack.display();
		// Is this try/catch really useful? We do nothing when an exception
		// is caught, the behavior is almost the same as without try/catch.
		try
		{
			_stateStack.handleInput();
		}
		catch(std::runtime_error& error)
		{
			std::cerr << "Exception caught: " << error.what();
			return -1;
		}
	}
	return 0;
}
