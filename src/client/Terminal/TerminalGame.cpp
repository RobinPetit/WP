// WizardPoker headers
#include "client/Terminal/TerminalGame.hpp"
#include "common/Deck.hpp"
#include "client/sockets/Client.hpp"
// std-C++ headers
#include <iostream>

///////////////////// init

TerminalGame::TerminalGame(Client& client):
	AbstractGame(client)
{
	std::cout << "Your game is about to start!\n";
}

void TerminalGame::chooseDeck()
{
	std::vector<Deck> decks = _client.getDecks();

	// Ask for the deck to use during the game
	for(std::size_t i{0}; i < decks.size(); i++)
		std::cout << i << " : " << decks.at(i).getName() << std::endl;
	int res = askIndex(decks.size(), "Choose your deck number: ");  // Chosen deck

	// Send the deck name to the server
	sf::Packet deckNamePacket;
	deckNamePacket << TransferType::GAME_PLAYER_GIVE_DECK_NAMES << decks.at(res).getName();
	_client.getGameSocket().send(deckNamePacket);
}

void TerminalGame::displayMessage(const std::string& message)
{
	std::cout << message << "\n";
}

void TerminalGame::displayAchievements(ClientAchievementList& newAchievements)
{
	if (newAchievements.size()==0)
		std::cout << "You have no new achievements" << std::endl;
	else
	{
		for (std::size_t i=0; i<newAchievements.size(); i++)
		{
			std::cout << " " << newAchievements.at(i).getPrettyName() << std::endl;
			std::cout << "   " << newAchievements.at(i).getDescription() << std::endl;
		}
	}
}

// TODO: rename this to be consistent
void TerminalGame::displayOptions()
{
	std::cout << "Here are your options:\n";
	std::size_t idx{0};
	for(const auto& action: _actions)
		std::cout << idx++ << ". " << action.first << std::endl;
}

void TerminalGame::startTurn()
{
	NonBlockingInput input;
	AbstractGame::startTurn();
	displayOptions();
	std::cout << "It is now your turn, what do you want to do? ";
	while(true)
	{
		if(not input.waitForData(0.1))
			continue;
		std::string command{input.receiveStdinData()};
		try
		{
			(this->*(_actions.at(std::stoi(command)).second))();
		}
		catch(std::logic_error&)
		{
			std::cout << "Your input should be in the range [" << 0 << ", " << _actions.size() << "[\n";
			continue;
		}
		if(not _myTurn.load())
			break;
		displayGame();
		displayOptions();
		std::cout << "What do you want to do next? ";
	}
}

//////////////////////// outputs

void TerminalGame::displayGame()
{
	std::lock_guard<std::mutex> _lock{_accessScreen};
	_client.getTerminal().clearScreen();
	std::cout << "***************" << std::endl;
	std::cout << "-----CARDS-----" << std::endl;
	std::cout << "-Cards in your deck: " << _selfDeckSize << std::endl;
	std::cout << "-Cards in your opponent's hand: " << _oppoHandSize << std::endl;
	std::cout << "-Cards in your graveyard:" << std::endl;
	displayCardVector(_selfGraveCards);
	std::cout << "-Cards on your opponent's board:" << std::endl;
	displayBoardCreatureVector(_oppoBoardCreatures, true);
	std::cout << "-Cards on your board:" << std::endl;
	displayBoardCreatureVector(_selfBoardCreatures, true);
	std::cout << "-Cards in your hand:" << std::endl;
	displayCardVector(_selfHandCards, true);

	std::cout << "-----HEALTH & ENERGY-----" << std::endl;
	std::cout << "-Opponent's health points: " << _oppoHealth << std::endl;
	std::cout << "-Your health points: " << _selfHealth << std::endl;
	std::cout << "-Your energy points: " << _selfEnergy << std::endl;
	std::cout << "***************" << std::endl;
}

void TerminalGame::displayCardVector(const std::vector<CardData>& cardVector, bool displayDescription)
{
	// Displays simple informations about a card vector
	for (auto i=0U; i<cardVector.size(); i++)
	{
		cardId id = cardVector.at(i).id;
		std::cout << "  * " << i << " : " << getCardName(id)
		          << " (cost: " << getCardCost(id) << ", "
		          << (isSpell(id) ? "spell" : "creature") << ")"
		          << (displayDescription ? "\n\t" + getCardDescription(id) : ",") << "\n";
	}
	std::cout << std::endl;
}

void TerminalGame::displayBoardCreatureVector(const std::vector<BoardCreatureData>& cardVector, bool displayDescription)
{
	// The board vector also contains real time informations about the cards (health, attack, shield, shield type)
	// This method should display these informations and be called only for displaying the board
	for (auto i=0U; i<cardVector.size(); i++)
	{
		const BoardCreatureData& thisCreature = cardVector.at(i);
		const cardId id = thisCreature.id;
		std::cout << "  * " << i << " : " << getCardName(id) << " (cost: " << getCardCost(id) <<
		             ", attack: " << thisCreature.attack <<
		             ", health: " << thisCreature.health <<
		             ", shield: " << thisCreature.shield << "-" << thisCreature.shieldType << ")"
		             << (displayDescription ? "\n\t" + getCardDescription(id) : "") << "\n";
	}
	std::cout << std::endl;
}

////////////// inputs

int TerminalGame::askIndex(std::size_t upperBound, const std::string& inputMessage)
{
	if (upperBound == 0)
	{
		std::cout << "There are no items to choose.\n";
		return -1;
	}

	int res = -1;
	std::cout << inputMessage;
	while(!(std::cin >> res) or res >= static_cast<int>(upperBound) or res < 0)
	{
		std::cin.clear();
		std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); //discard characters until newline is found
		std::cout << "Your answer should be in the range [" << 0 << ", " << upperBound <<"[ !\n" << inputMessage;
	}

	return res;
}

int TerminalGame::askSelfHandIndex()
{
	std::cout << "These are the cards in your hand:" << std::endl;
	displayCardVector(_selfHandCards, true);
	return askIndex(_selfHandCards.size(), "Choose the index for a card in your hand: ");
}

int TerminalGame::askSelfBoardIndex()
{
	std::cout << "These are the cards on your board:" << std::endl;
	displayBoardCreatureVector(_selfBoardCreatures, true);
	return askIndex(_selfBoardCreatures.size(), "Choose the index for a card on your board: ");
}

int TerminalGame::askSelfGraveyardIndex()
{
	std::cout << "These are the cards in your graveyard:" << std::endl;
	displayCardVector(_selfGraveCards, true);
	return askIndex(_selfGraveCards.size(), "Choose the index for a card in the graveyard: ");
}

int TerminalGame::askOppoHandIndex()
{
	std::cout << "Your opponent has " << _oppoHandSize << " cards in his hand." << std::endl;
	return askIndex(_oppoHandSize, "Choose the index for a card in the opponent's hand: ");
}

int TerminalGame::askOppoBoardIndex()
{
	std::cout << "These are the cards on your opponent's board:" << std::endl;
	displayBoardCreatureVector(_oppoBoardCreatures, true);
	return askIndex(_oppoBoardCreatures.size(), "Choose the index for a card on the opponent's board: ");
}

bool TerminalGame::wantToAttackOpponent()
{
	std::string buffer;
	std::cout << "Do you want to attack the opponent (y for \"yes\", anything else for \"no\")?\n\t";
	std::cin.ignore();
	std::getline(std::cin, buffer);
	return buffer == "y";
}

void TerminalGame::receiveCard(cardId id)
{
	displayMessage("You won the card '" + getCardName(id) + "'");
}

TerminalGame::~TerminalGame()
{

}
