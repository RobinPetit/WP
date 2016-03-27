// std-C++ headers
#include <iostream>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/StateStack.hpp"
#include "client/states/AbstractDecksManagementState.hpp"

AbstractDecksManagementState::AbstractDecksManagementState(Context& context):
	AbstractState(context)
{
	try
	{
		_decks = _context.client->getDecks();
		_cardsCollection = _context.client->getCardsCollection();
	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Default card collection loaded.\n";
	}
}

void AbstractDecksManagementState::backMainMenu()
{
	_context.stateStack->pop();
}

std::vector<CardId> AbstractDecksManagementState::getCardsCollectionWithoutDeck(const Deck& deck) const
{
	std::vector<CardId> res;
	std::copy(_cardsCollection.cbegin(), _cardsCollection.cend(), std::back_inserter(res));

	// Remove all cards that are in the deck from the result
	for(auto cardInDeck : deck)
	{
		// Remove the card in the deck from the result
		auto it(std::find(res.begin(), res.end(), cardInDeck));
		if(it != res.end())
			res.erase(it);

		// Remove all extra cards from the result: if the deck contains one time
		// a given card, there is no need to display more than one time this card in
		// the result, because the user won't be able to have this cards three time
		// in the deck. The same applies if there is two times a card in the deck.
		const std::size_t numberInDeck{static_cast<std::size_t>(std::count(deck.begin(), deck.end(), cardInDeck))};
		while(static_cast<std::size_t>(std::count(res.begin(), res.end(), cardInDeck)) > 2 - numberInDeck)
			res.erase(std::find(res.begin(), res.end(), cardInDeck));
	}
	return res;
}
