/**
    server entry point
**/

// WizardPoker common headers
#include <common/constants.hpp>
// WizardPoker server headers
#include <server/Server.hpp>

int main()
{
	return Server().start(SERVER_PORT);
}
