#include <iostream>
#include "client/states/CardsCollectionState.hpp"

CardsCollectionState::CardsCollectionState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Back to main menu", &CardsCollectionState::backMainMenu);
    //Get the list of cards from the server...
    _cards.emplace_back("Saruman the White");
    _cards.emplace_back("Gandalf the Grey");
    _cards.emplace_back("Aragorn, heir of Isildur");
    _cards.emplace_back("Legolas, son of Thranduil");
    _cards.emplace_back("Gimli, son of Glóin");
    _cards.emplace_back("Boromir, son of Denethor II");
}

void CardsCollectionState::display()
{
    std::cout << "Here are your cards:\n";
    for(const auto& card : _cards)
		std::cout << "* " << card << "\n";
	std::cout << std::string(40, '*') << "\n";

    // Display the actions
    AbstractState::display();
}

void CardsCollectionState::backMainMenu()
{
    stackPop();
}


