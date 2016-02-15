#include <iostream>
#include "client/states/DecksManagementState.hpp"

DecksManagementState::DecksManagementState(StateStack& stateStack):
	AbstractState(stateStack)
{
	addAction("Back to main menu", &DecksManagementState::backMainMenu);
	addAction("Display a deck", &DecksManagementState::displayDeck);
	addAction("Edit a deck", &DecksManagementState::editDeck);
	addAction("Create a deck", &DecksManagementState::createDeck);
	addAction("Delete a deck", &DecksManagementState::deleteDeck);
	// Get the decks from the server...
	_decks.emplace_back("Aggro deck");
	_decks.emplace_back("Defense deck");
}

void DecksManagementState::display()
{
	std::cout << "Here are your decks:\n";
	int i{0};
	for(const auto& deck : _decks)
		std::cout << ++i << ". " << deck << "\n";
	std::cout << std::string(40, '*') << "\n";

	// Display the actions
	AbstractState::display();
}

void DecksManagementState::displayDeck()
{
	if(_decks.empty())
	{
		std::cout << "There are no deck to display!\n";
		return;
	}
	std::cout << "Which deck would you like to display (insert a number in [1, " << _decks.size() << "])? ";
		const int input{askForNumber(1UL, _decks.size() + 1) - 1};
	// Here get the content of the deck _decks.at(input)...
	std::vector<std::string> cards = {"Our", "Wizard", "Poker", "Is", "The", "Best"};
	for(const auto& card : cards)
		std::cout << "* " << card << "\n";
}

void DecksManagementState::editDeck()
{
	if(_decks.empty())
	{
		std::cout << "There are no deck to edit!\n";
		return;
	}
	try
	{
		std::cout << "Which deck would you like to edit (insert a number in [1, " << _decks.size() << "])? ";
		const int input{askForNumber(1UL, _decks.size() + 1) - 1};
		std::cout << "Ok, modifying deck \"" << _decks.at(input) << "\"...\n";
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
	if(_decks.empty())
	{
		std::cout << "There are no deck to delete!\n";
		return;
	}
	try
	{
		std::cout << "Which deck would you like to delete (insert a number in [1, " << _decks.size() << "])? ";
		const int input{askForNumber(1UL, _decks.size() + 1) - 1};
		_decks.erase(_decks.begin() + input);
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

int DecksManagementState::askForNumber(int from, int to)
{
	std::string input;
	std::getline(std::cin, input);
	// Get a number from the user input
	const int intInput{std::stoi(input)};
	if(intInput < from or intInput >= to)
		throw std::out_of_range("Index out of range.");
	return intInput;
}


