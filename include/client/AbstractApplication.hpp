#ifndef _ABSTRACT_APPLICATION_CLIENT_HPP
#define _ABSTRACT_APPLICATION_CLIENT_HPP

// std-C++ headers
#include <memory>  // std::unique_ptr
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/Context.hpp"
// Although AbstractState is not used in this file, this include is necessary
// to solve a cyclic dependencies issue
#include "client/AbstractState.hpp"
#include "client/StateStack.hpp"

/// Generic class holding variables that can be used by both GUI and terminal.
class AbstractApplication
{
	public:
		/// Constructor.
		AbstractApplication();

		/// Destructor.
		virtual ~AbstractApplication() = default;

		/// Play the game.
		/// \return The return status of the application.
		virtual int play() = 0;

	protected:
		Client _client;          ///< The client.
		StateStack _stateStack;  ///< The state stack.
		Context _context;        ///< The context.
};

#endif  // _ABSTRACT_APPLICATION_CLIENT_HPP
