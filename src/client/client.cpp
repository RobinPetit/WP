#include "client/StateStack.hpp"
#include "client/states/MainMenuState.hpp"

int main()
{
    StateStack s;
    s.push<MainMenuState>();
    while(not s.isEmpty())
    {
        s.display();
		std::string input;
		std::getline(std::cin, input);
        s.handleEvent(input);
    }
    return 0;
}
