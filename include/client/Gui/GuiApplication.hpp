#ifndef _GUI_APPLICATION_CLIENT_HPP
#define _GUI_APPLICATION_CLIENT_HPP

// External headers
#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/Gui.hpp>
// WizardPoker headers
#include "client/AbstractApplication.hpp"

/// Main class of the GUI front-end.
class GuiApplication : public AbstractApplication
{
	public:
		/// Constructor.
		GuiApplication();

		/// Playe the game.
		virtual int play() override;

	private:
		sf::VideoMode _videoMode;
		std::string _windowTitle;
		sf::RenderWindow _window;
		tgui::Gui _gui;
};

#endif  // _GUI_APPLICATION_CLIENT_HPP
