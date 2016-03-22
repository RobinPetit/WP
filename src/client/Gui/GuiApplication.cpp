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
	_window.setKeyRepeatEnabled(false);
	_window.setFramerateLimit(60);
}

int GuiApplication::play()
{
	_stateStack.firstPush<GuiHomeState>();
	try
	{
		while(not _stateStack.isEmpty())
		{
			_stateStack.display();
			_stateStack.handleInput();
		}
	}
	catch(std::runtime_error& error)
	{
		std::cerr << "Exception caught: " << error.what();
		return -1;
	}
	return 0;
}
