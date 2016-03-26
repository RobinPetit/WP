/// Client main file (entry point).

// WizardPoker headers
#include "client/Terminal/TerminalApplication.hpp"
#include "client/Gui/GuiApplication.hpp"

int main(int argc, char* argv[])
{
	if(argc > 1 and std::string(argv[1]) == "--no-gui")
		return TerminalApplication().play();
	else
		return GuiApplication().play();
}
