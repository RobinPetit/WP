#include "client/StateStack.hpp"
#include "client/states/MainMenuState.hpp"

int main()
{
	StateStack s;
	s.push<MainMenuState>();
	while(not s.isEmpty())
	{
		s.draw();
		s.handleEvent();
		s.update();
	}
	return 0;
}
