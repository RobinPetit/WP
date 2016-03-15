/// Client main file (entry point).

// std-C++ headers
#include <memory>
// WizardPoker headers
#include "client/Terminal/TerminalApplication.hpp"
#include "client/Gui/GuiApplication.hpp"

int main(/* int argc, char* argv[] */)
{
	std::unique_ptr<AbstractApplication> application;
	// TODO: uncomment the following line when the GUI is working
	if(true /* argc > 1 and std::string(argv[1]) == "--no-gui" */)
		application.reset(new TerminalApplication());
	else
		application.reset(new GuiApplication());

	return application->play();
}
