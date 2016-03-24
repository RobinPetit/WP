/**
	chat main file (entry point)
**/

// WizardPoker headers
#include "chat/Terminal/TerminalChat.hpp"
#include "chat/Gui/GuiChat.hpp"
// std-C++ headers
#include <cassert>
#include <iostream>
#include <memory>

int main(int argc, char **argv)
{
	assert(argc == 7);
	std::unique_ptr<AbstractChat> chatManager;
	if(std::string(argv[6]) == "gui")
		chatManager.reset(new GuiChat(argv));
	else
		chatManager.reset(new TerminalChat(argv));
	chatManager->start();

	return EXIT_SUCCESS;
}
