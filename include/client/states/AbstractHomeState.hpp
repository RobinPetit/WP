#ifndef _ABSTRACT_HOME_STATE_CLIENT_HPP
#define _ABSTRACT_HOME_STATE_CLIENT_HPP

// std-C++ headers
#include <string>
#include <utility>
// SFML headers
#include <SFML/Network/Packet.hpp>  // sf::Uint16
// WizardPoker headers
#include "client/AbstractState.hpp"

/// Abstract version of the home state.
class AbstractHomeState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractHomeState(Context& context);

	protected:
		/// Quits the application.
		void quit();

		/// Tries to connect the client to the server.
		/// \throw UnableToConnectException if the connection did not succeeded.
		/// \post The connection is established.
		void tryToConnect(const std::string& userName, const std::string& password);

		/// Tries to register a new user to the server.
		/// \throw std::runtime_error if an error occurred.
		/// \post The user is registered.
		void tryToRegister(const std::string& userName, const std::string& password);

	private:
		/// Reads the configuration file and return the address and the port of
		/// the server.
		/// \return A tuple where first is the IP address and second is the port.
		/// \throw std::runtime_error if an error occurred (configuration file
		/// not available, needed data not available in the configuration file).
		static std::pair<std::string, sf::Uint16> getConnectionConfiguration();
};

#endif// _ABSTRACT_HOME_STATE_CLIENT_HPP
