/**
    server entry point
**/

// SFML headers
#include <SFML/Network.hpp>
// WizardPoker common headers
#include <common/constants.hpp>
// WizardPoker server headers
#include <server/Server.hpp>

int main()
{
	return Server().start(SERVER_PORT);
}
