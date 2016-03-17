// WizardPoker headers
#include "chat/Terminal/TerminalChat.hpp"
// std-C++ headers
#include <iostream>
#include <chrono>
// SFML headers
#include <SFML/Network/Packet.hpp>

TerminalChat::TerminalChat(const char * const argv[6]):
	AbstractChat(argv)
{

}

void TerminalChat::output()
{
	std::string message;
	while(_running)
	{
		std::getline(std::cin, message);
		treatMessage(message);
	}
}

void TerminalChat::display(const std::string& name, const std::string& message)
{
	std::string toDisplay{(not _friendPresence.load() ? "// " : "") + _terminal.setBold(name) + ": " + message};
	std::cout << toDisplay << std::endl;
}
