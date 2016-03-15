#ifndef _GUI_APPLICATION_CLIENT_HPP
#define _GUI_APPLICATION_CLIENT_HPP

// SFML headers
#include <SFML/Graphics/RenderWindow.hpp>
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
};

#endif  // _GUI_APPLICATION_CLIENT_HPP
