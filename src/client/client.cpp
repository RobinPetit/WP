#include <iostream>
#include "client/StateStack.hpp"
#include "client/states/HomeState.hpp"

int main()
{
    StateStack s;
    s.push<HomeState>();
    while(not s.isEmpty())
    {
        s.display();
		std::string input;
		std::getline(std::cin, input);
        s.handleInput(input);
    }
    return 0;
}
