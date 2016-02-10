#include <iostream>
#include <cstdlib>
#include "client/StateStack.hpp"
#include "client/states/HomeState.hpp"

int main()
{
    StateStack s;
    s.push<HomeState>();
    while(not s.isEmpty())
    {
		system("clear");
        s.display();
        std::cout << "What do you want to do? ";
		std::string input;
		std::getline(std::cin, input);
        s.handleInput(input);
    }
    return 0;
}
