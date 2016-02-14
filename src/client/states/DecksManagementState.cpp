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
    std::cout << "Which deck would you like to display (insert a number in [1, " << _decks.size() << "])? ";
    const int index{askForNumber(1, _decks.size() + 1) - 1};
    for(const auto& card : _decks[index])  // Browse the deck
        std::cout << "* " << card << "\n";
}

void DecksManagementState::editDeck()
{
    if(_decks.empty())
    {
        std::cout << "There are no deck to edit!\n";
        return;
    }
    std::cout << "Which deck would you like to edit (insert a number in [1, " << _decks.size() << "])? ";
    int deckIndex, replacedIndex;
    try
    {
        deckIndex = askForNumber(1, _decks.size() + 1) - 1;
    }
    catch(const std::logic_error& e)
    {
        std::cout << "Wrong deck index!\n";
        return;
    }
    do
    {
        try
        {
            // Ask for the replacee card
            std::cout << "Content of your card collection:\n";
            int i{0};
            for(const auto& card : _cardsCollection)
                std::cout << ++i << ". " << card << "\n";
            std::cout << "Which card do you want to put in you deck? ";
            int replacee{askForNumber(1, _cardsCollection.getSize() + 1) - 1};

            i = 0;
            // Ask for the replaced cardgit
            std::cout << "Content of the deck " << _decks[deckIndex].getName() << ":\n";
            for(const auto& card : _decks[deckIndex])
                std::cout << ++i << ". " << card << "\n";
            std::cout << "Which card do you want to replace (0 to quit)? ";
            replacedIndex = askForNumber(0, Deck::size + 1);
            if(replacedIndex > 0)
            {
                Card::ID replaceeCard{static_cast<Card::ID>(replaceeCard)};
                if(std::count(_decks[deckIndex].begin(), _decks[deckIndex].end(), (replaceeCard)) > 1)
                    std::cout << replaceeCard << " is already two times in your deck, this is the maximum.\n";
                else
                    _decks[deckIndex].changeCard(replacedIndex, replaceeCard);
            }
        }
        catch(const std::out_of_range& e)
        {
            std::cout << "Wrong card index!\n";
            replacedIndex = 1;
        }
    } while(replacedIndex != 0);
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
        std::cout << "Which deck would you like to delete (insert a number in [1, " << _decks.size() << "])? ";
        const int input{askForNumber(1, _decks.size() + 1) - 1};
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


