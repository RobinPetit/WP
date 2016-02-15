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
    _decks.emplace_back("Default deck");
    _decks.emplace_back("Aggro deck");
    _decks.emplace_back("Defense deck");
    // And the card collection too (for now this is the default card collection)
}

void DecksManagementState::display()
{
    std::cout << "Here are your decks:\n";
    int i{0};
    for(const auto& deck : _decks)
        std::cout << ++i << ". " << deck.getName() << "\n";
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
    std::cout << "Which deck would you like to display? ";
    const std::size_t index{askForNumber(1, _decks.size() + 1) - 1};
    for(const auto& card : _decks[index])  // Browse the deck
        std::cout << "* " << card << "\n";
    waitForEnter();
}

void DecksManagementState::editDeck()
{
    if(_decks.empty())
    {
        std::cout << "There are no deck to edit!\n";
        return;
    }
    std::cout << "Which deck would you like to edit ? ";
    std::size_t deckIndex;
    try
    {
        deckIndex = askForNumber(1, _decks.size() + 1) - 1;
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong deck index!\n";
        return;
    }
    // N + 1/2 loop (stop as soon as the user enters 0).
    // askForReplacedCard calls must be in a try block, so it must be
    // inside the loop (we cannot write `statement; while(condition) statement;`).
	while(true)
	{
        try
        {
			std::size_t replacedIndex = askForReplacedCard(deckIndex);
			if(replacedIndex == 0)
				break;
			// Indices are displayed 1 greater than their actual values
			// (for the card 0, the index 1 is shown to the user)
			--replacedIndex;
            // Ask for the replacing card put it in the deck
			_decks[deckIndex].changeCard(replacedIndex, askForReplacingCard(deckIndex));
        }
        catch(const std::out_of_range& e)
        {
            std::cout << "Wrong card index!\n";
        }
        catch(const std::logic_error& e)
        {
			std::cout << e.what() << "\n";
		}
    };
}
std::size_t DecksManagementState::askForReplacedCard(std::size_t deckIndex)
{
	int i{0};
	std::cout << "Content of the deck " << _decks[deckIndex].getName() << ":\n";
	for(const auto& card : _decks.at(deckIndex))
		std::cout << ++i << ". " << card << "\n";
	std::cout << "Which card do you want to replace (0 to quit)? ";
	return askForNumber(0, Deck::size + 1);
}

Card::ID DecksManagementState::askForReplacingCard(std::size_t deckIndex)
{
	std::cout << "Content of your card collection:\n";
	for(const auto& card : _cardsCollection)
		std::cout << static_cast<std::size_t>(card) << ". " << card << "\n";
	std::cout << "Which card do you want to put in you deck? ";
	// \TODO Replace 666 by the number of different cards in the game
	Card::ID replacingCard{askForNumber(0, 666)};

	// Check if the given card is valid
	if(not _cardsCollection.contains(replacingCard))
		throw std::logic_error("You do not have this card!");
	if(std::count(_decks.at(deckIndex).begin(), _decks.at(deckIndex).end(), replacingCard) > 1)
		throw std::logic_error(std::to_string(replacingCard) + " is already two times in your deck, this is the maximum allowed.");

	return replacingCard;
}

void DecksManagementState::createDeck()
{
    std::cout << "What is the name of your new deck? ";
    std::string input;
    std::getline(std::cin, input);
    _decks.emplace_back(input);
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
        std::cout << "Which deck would you like to delete? ";
        const std::size_t input{askForNumber(1, _decks.size() + 1) - 1};
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

std::size_t DecksManagementState::askForNumber(std::size_t from, std::size_t to)
{
    std::string input;
    std::getline(std::cin, input);
    // Get a number from the user input
    const std::size_t intInput{std::stoul(input)};
    if(intInput < from or intInput >= to)
        throw std::out_of_range("Index out of range.");
    return intInput;
}
