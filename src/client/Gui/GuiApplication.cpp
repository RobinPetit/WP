// WizardPoker headers
#include "client/Gui/states/GuiHomeState.hpp"
#include "client/Gui/GuiApplication.hpp"

GuiApplication::GuiApplication():
	AbstractApplication(true),
	_videoMode{1000, 700},
	_windowTitle{"Wizard Poker"},
	_window{_videoMode, _windowTitle, sf::Style::Titlebar | sf::Style::Close},
	_gui{_window},
	_backgroundPicture{std::make_shared<tgui::Picture>("../resources/client/background.jpg")}
{
	_context.window = &_window;
	_context.gui = &_gui;
	_window.setKeyRepeatEnabled(false);
	_window.setFramerateLimit(60);
	_gui.add(_backgroundPicture);
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
