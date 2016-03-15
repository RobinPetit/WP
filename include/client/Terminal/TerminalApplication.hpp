#ifndef _TERMINAL_APPLICATION_CLIENT_HPP
#define _TERMINAL_APPLICATION_CLIENT_HPP

// WizardPoker headers
#include "client/AbstractApplication.hpp"

/// Main class of the terminal front-end.
class TerminalApplication : public AbstractApplication
{
	public:
		/// Playe the game.
		virtual int play() override;
};

#endif  // _TERMINAL_APPLICATION_CLIENT_HPP
