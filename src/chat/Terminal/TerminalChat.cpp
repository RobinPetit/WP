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
	std::string input;
	std::wstring message;
	while(_running)
	{
		std::getline(std::cin, input);
		message = std::wstring(input.begin(), input.end());
		treatMessage(message);
	}
}

void TerminalChat::display(const std::string& name, const std::wstring& message)
{
	std::string boldName{_terminal.setBold(name)};
	std::wstring tmpWide{(not _friendPresence.load() ? L"// " : L"") + std::wstring(boldName.begin(), boldName.end()) + L": " + message};
	std::string toDisplay(tmpWide.begin(), tmpWide.end());
	std::cout << toDisplay << std::endl;
}
