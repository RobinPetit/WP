#ifndef _GUI_APPLICATION_CLIENT_HPP
#define _GUI_APPLICATION_CLIENT_HPP

// External headers
#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/Picture.hpp>
// WizardPoker headers
#include "client/AbstractApplication.hpp"

/// Main class of the GUI front-end.
class GuiApplication : public AbstractApplication
{
	public:
		/// Constructor.
		GuiApplication();

		/// Play the game.
		/// \return The return status of the application.
		virtual int play() override;

	private:
		const sf::VideoMode _videoMode;         ///< The video mode (i.e. the resolution).
		const std::string _windowTitle;         ///< The title of the window.
		sf::RenderWindow _window;               ///< The window.
		tgui::Gui _gui;                         ///< The main GUI class.
		tgui::Picture::Ptr _backgroundPicture;  ///< The background image.
};

#endif  // _GUI_APPLICATION_CLIENT_HPP
