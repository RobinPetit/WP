#include "client/StateStack.hpp"
#include "clients/states/MainMenuState.hpp"

int main()
{
	StateStack s;
	s.pushState<MainMenuState>();
	while(true)
	{
		s.handleEvent();
		s.update();
		s.draw();
	}
	return 0;
}
