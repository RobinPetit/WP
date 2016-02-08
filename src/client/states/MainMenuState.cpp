#include <client/states/MainMenuState.hpp>

MainMenuState::MainMenuState()
{
}

void MainMenuState::draw()
{
	std::cout << "Hi, this is the main menu, what do you want to do?\n";
}

bool MainMenuState::update()
{
	return true;
}

bool MainMenuState::handleEvent()
{
	std::string input;
	std::getline(std::cin, input);
	//Handle input...
	return false;
}

