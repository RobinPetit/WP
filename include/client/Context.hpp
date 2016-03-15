#ifndef _CONTEXT_CLIENT_HPP
#define _CONTEXT_CLIENT_HPP

// SFML headers
#include <SFML/Graphics/RenderWindow.hpp>

// Forward declarations
class Client;
class StateStack;

/// Struct used to hold pointers that need to be passed to the
/// states. This struct even holds pointers used by only the GUI and
/// others only by the terminal. The reason is that this is the simpler
/// design I found, making subclasses of Context for GUI and terminal
/// lead to very, very complex code. I made the choice of a design
/// less clean but way simpler.
///
/// We are using pointers because not all members will be initialized:
/// if we are making a terminal game, the window attribute has nothing
/// to point to.
///
/// This struct has public attributes because it have no logic (methods),
/// its only mean is to hold references.
struct Context
{
	// Common attributes - keep them first!
	Client* client;            ///< The client.
	StateStack* stateStack;    ///< The state stack.

	// GUI attributes
	sf::RenderWindow* window;  ///< The window.
	// tgui::Gui* gui;            ///< The GUI.
};

#endif  // _CONTEXT_CLIENT_HPP
