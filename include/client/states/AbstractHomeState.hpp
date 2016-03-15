#ifndef _ABSTRACT_HOME_STATE_CLIENT_HPP
#define _ABSTRACT_HOME_STATE_CLIENT_HPP

#include <string>
#include <utility>
#include "client/AbstractState.hpp"

// Forward declarations
class StateStack;

/// Cannot be more explicit.
class AbstractHomeState : virtual public AbstractState
{
	public:
		/// Constructor.
		AbstractHomeState(StateStack& stateStack, Client& client);

		/// The display function.
		/// It must do all things related to drawing or printing stuff on the screen.
		virtual void display() override;

	private:
		void connect();
		void createAccount();
		void quit();
		static std::pair<std::string, std::string> askIdentifiers();
};

#endif// _ABSTRACT_HOME_STATE_CLIENT_HPP
