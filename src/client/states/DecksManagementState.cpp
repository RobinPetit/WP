#include <iostream>
#include "client/states/DecksManagementState.hpp"

DecksManagementState::DecksManagementState(StateStack& stateStack):
    AbstractState(stateStack)
{
    addAction("Edit a deck", &DecksManagementState::editDeck);
    addAction("Create a deck", &DecksManagementState::createDeck);
    addAction("Delete a deck", &DecksManagementState::deleteDeck);
    addAction("Back to main menu", &DecksManagementState::backMainMenu);
    // Get the decks from the server...
    _decks.emplace_back("Aggro deck");
    _decks.emplace_back("Defense deck");
}

void DecksManagementState::display()
{
    std::cout << "Here are your decks:\n";
    unsigned int i{0};
    for(const auto& deck : _decks)
		std::cout << ++i << ". " << deck << "\n";
	std::cout << std::string(40, '*') << "\n";
	i = 0;
    // Display the actions
    for(const auto& pair : _actions)
        std::cout << ++i << ". " << pair.first << "\n";
    std::cout << "What do you want to do? ";
}

void DecksManagementState::handleInput(const std::string& input)
{
    try
    {
        // Get a number from the user input
        const int intInput{std::stoi(input) - 1};
        // Call the method at index intInput
        // std::vector::at throws std::out_of_range if intInput is out of bounds
        _actions.at(intInput).second();
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong input!\n";
    }
}

void DecksManagementState::editDeck()
{
    try
    {
		std::cout << "What deck would you modify (insert a number in [1, " << _decks.size() << "])? ";
		std::string input;
		std::getline(std::cin, input);
        // Get a number from the user input
        const int intInput{std::stoi(input) - 1};
        std::cout << "Ok, modifying deck \"" << _decks.at(intInput) << "\"...\n";
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong input!\n";
    }
}

void DecksManagementState::createDeck()
{
	std::cout << "What is the name of your new deck? ";
	std::string input;
	std::getline(std::cin, input);
	_decks.push_back(input);
}

void DecksManagementState::deleteDeck()
{
    try
    {
		std::cout << "What deck would you delete (insert a number in [1, " << _decks.size() << "])? ";
		std::string input;
		std::getline(std::cin, input);
        // Get a number from the user input
        const int intInput{std::stoi(input) - 1};
        if(intInput < 0 or intInput >= _decks.size())
			throw std::out_of_range("Deck index out of range.");
        _decks.erase(_decks.begin() + intInput);
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong input!\n";
    }
}

void DecksManagementState::backMainMenu()
{
    stackPop();
}


