#include "client/StateStack.hpp"
#include "client/states/MainMenuState.hpp"

int main()
{
	StateStack s;
	s.push<MainMenuState>();
	while(not s.isEmpty())
	{
		s.display();
		s.handleEvent();
	}
	return 0;
}
