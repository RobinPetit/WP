/**
	chat main file (entry point)
**/

// WizardPoker headers
#include "chat/Terminal/TerminalChat.hpp"
// std-C++ headers
#include <cassert>
#include <iostream>

int main(int argc, char **argv)
{
	assert(argc == 6);

	// TODO: give one more parameter to know if we need to use GUIChat or TerminalChat
	TerminalChat chat(argv);
	chat.start();

	return EXIT_SUCCESS;
}
