/// Client main file (entry point).

// std-C++ headers
#include <memory>
// WizardPoker headers
#include "client/Terminal/TerminalApplication.hpp"
#include "client/Gui/GuiApplication.hpp"

int main(int argc, char* argv[])
{
	std::unique_ptr<AbstractApplication> application;
	// TODO for now we must give --gui, but eventually this will be inverted,
	// and we'll have to give --no-gui.
	if(argc > 1 and std::string(argv[1]) == "--gui")
		application.reset(new GuiApplication());
	else
		application.reset(new TerminalApplication());

	return application->play();
}
