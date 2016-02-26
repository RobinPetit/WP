// std-C++ headers
#include <iostream>
#include <cstdlib>
#include <cassert>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "common/Terminal.hpp"
#include "common/ini/IniFile.hpp"
#include "client/states/GameState.hpp"
#include "client/NonBlockingInput.hpp"

GameState::GameState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client),
	_playing(true)
{
	addAction("Quit", &GameState::quit);
	addAction("Use a card from hand", &GameState::useCard);
	addAction("Attack with a creature", &GameState::attackWithCreature);
	addAction("End your turn", &GameState::endTurn);
	std::cout << "Your game is about to start!\n";
	_client.waitTillReadyToPlay();
	init();
	play();
}

void GameState::init()
{
	initListening();
	chooseDeck();
	sf::Packet packet;
	_client.getGameSocket().receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::GAME_STARTING)
		throw std::runtime_error("Wrong signal received: " + std::to_string(static_cast<sf::Uint32>(type)));
	packet >> type;
	if(type == TransferType::GAME_PLAYER_ENTER_TURN)
		_myTurn.store(true);
	else if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
		_myTurn.store(false);
	else
		throw std::runtime_error("Wrong turn information: " + std::to_string(static_cast<sf::Uint32>(type)));
}

void GameState::chooseDeck()
{
	std::vector<Deck> decks = _client.getDecks();
	// ask for the deck to use during the game
	std::cout << "Choose your deck:\n\t";
	for(auto i{0U}; i < decks.size(); ++i)
		std::cout << i+1 << ": " << decks[i].getName() << std::endl;
	std::cin.clear();
	sf::Uint32 chosenDeck;
	bool loop{true};
	do
	{
		try
		{
			chosenDeck = askForNumber(1, decks.size()+1)-1;
			loop = false;
		}
		catch(std::exception& e)
		{
			std::cout << "exception: " << e.what();
		}
	}
	while(loop);
	std::cout << "sending your deck...\n";
	// send the deck ID to the server
	sf::Packet deckIdPacket;
	deckIdPacket << TransferType::GAME_PLAYER_GIVE_DECK_ID << chosenDeck;
	_client.getGameSocket().send(deckIdPacket);
}

void GameState::play()
{
        while(_playing.load())
        {
		if(_myTurn.load())
			startTurn();
		else
		{
			std::cout << "Waiting for your turn\n";
			while(not _myTurn.load())
				sf::sleep(sf::milliseconds(100));
		}
        }
}

void GameState::display()
{
	std::cout << "Here are your options:\n";
	// Display the actions
	AbstractState::display();
}

void GameState::startTurn()
{
	_myTurn.store(true);
	display();
	std::cout << "It is now your turn, type something\n";
	while(_myTurn.load())
	{
		if(not _nonBlockingInput.waitForData(0.1))
			continue;
		std::string command{_nonBlockingInput.receiveStdinData()};
		std::cout << "handling command '" << command << "'\n";
		handleInput(command);
		std::cout << "handled\n";
	}
	/**/
}


//PRIVATE METHODS

// Request user for additional input
int GameState::askIndex(std::size_t maxIndex, std::string inputMessage)
{
	std::size_t res; //result
	if (maxIndex==0)
	{
		std::cout << "There are no cards to choose\n";
		return -1;
	}

	do
	{
		std::cout << inputMessage;
		std::cin >> res;
		if(res > maxIndex)
			std::cout << "Your answer should be in the range (" << 0 << ", " << maxIndex <<") !\n";
	}while(res>maxIndex);

	return res;
}

int GameState::askSelfHandIndex()
{
	std::cout << "These are the cards in your hand:" << std::endl;
	displayCardVector(_selfHandCards);
	return askIndex(_selfHandCards.size(), "Choose the index for a card in your hand :");
}

int GameState::askSelfBoardIndex()
{
	std::cout << "These are the cards on your board:" << std::endl;
	displayBoardCreatureVector(_selfBoardCreatures);
	return askIndex(_selfBoardCreatures.size(), "Choose the index for a card on your board :");
}

int GameState::askSelfGraveyardIndex()
{
	std::cout << "These are the cards in your graveyard:" << std::endl;
	displayCardVector(_selfGraveCards);
	return askIndex(_selfGraveCards.size(), "Choose the index for a card in the graveyard :");
}

int GameState::askOppoHandIndex()
{
	std::cout << "Your opponent has " << _oppoHandSize << " cards in his hand." << std::endl;
	return askIndex(_oppoHandSize, "Choose the index for a card in the opponent's hand :");
}

int GameState::askOppoBoardIndex()
{
	std::cout << "These are the cards on your opponent's board:" << std::endl;
	displayBoardCreatureVector(_oppoBoardCreatures);
	return askIndex(_oppoBoardCreatures.size(), "Choose the index for a card on the opponent's board :");
}

//User actions
void GameState::useCard()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		int cardIndex = askSelfHandIndex();
		std::cout << "You want to use card" << cardIndex << "\n";
		//NETWORK: USE_CARD
	}
}

void GameState::attackWithCreature()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::cout << "Which creature would you like to attack with?\n";
		int selfCardIndex = askSelfBoardIndex();

		std::cout << "Which opponent's creature would you like to attack?\n";
		int oppoCardIndex = askOppoBoardIndex();

		///If there's cards on board
		if(selfCardIndex >= 0 && oppoCardIndex >= 0)
		{
			//NETWORK: ATTACK WITH CREATURE
		}
		else
		{
			//do something ?
		}
	}
}


void GameState::endTurn()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		_myTurn = false;
	}
}

void GameState::quit()
{
	_playing.store(true);
	_listeningThread.join();
	stackPop();
}

GameState::~GameState()
{
	quit();
}

void GameState::displayGame()
{
	_client.getTerminal().clearScreen();
	std::cout << "-----CARDS-----" << std::endl;
	std::cout << "You have " << _selfDeckSize << " cards left in your deck." << std::endl;
	std::cout << "Your opponent has " << _oppoHandSize << " cards in his hand." << std::endl;
	std::cout << "Cards in your graveyard:" << std::endl;
	displayCardVector(_selfGraveCards);
	std::cout << "Cards on your opponent's board:" << std::endl;
	displayBoardCreatureVector(_oppoBoardCreatures);
	std::cout << "Cards on your board:" << std::endl;
	displayBoardCreatureVector(_selfBoardCreatures);
	std::cout << "Cards in your hand:" << std::endl;
	displayCardVector(_selfHandCards);

	std::cout << "-----HEALTH & ENERGY-----" << std::endl;
	std::cout << "Your opponent has " << _oppoHealth << " life points left." << std::endl;
	std::cout << "You have " << _selfHealth << " life points left." << std::endl;
	std::cout << "You have " << _selfEnergy << " energy points left." << std::endl;
}

void GameState::displayCardVector(std::vector<CardData> cardVector)
{
	for (auto i=0U; i<cardVector.size(); i++)
	{
		cardId id = cardVector.at(i).id;
		std::cout << i << " : " << getCardName(id) << "(cost:" << getCardCost(id) << ")";
		//TODO use card names instead of card IDs ?
		if (i!=cardVector.size()-1)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

void GameState::displayBoardCreatureVector(std::vector<BoardCreatureData> cardVector)
{
	// The board vector also contains real time informations about the cards (health, attack, shield, shield type)
	// This method should display these informations and be called only for displaying the board
	for (auto i=0U; i<cardVector.size(); i++)
	{
		BoardCreatureData thisCreature = cardVector.at(i);
		cardId id = cardVector.at(i).id;
		std::cout << i << " : " << getCardName(id) << "(cost:" << getCardCost(id) <<
		          ", attack:" << thisCreature.attack <<
		          ", health:" << thisCreature.health <<
		          ", shield:" << thisCreature.shield << "-" << thisCreature.shieldType << ")";
		//TODO use card names instead of card IDs ?
		if (i!=cardVector.size()-1)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

//////////////// special data receival

void GameState::initListening()
{
	_listeningThread = std::thread(&GameState::inputListening, this);
}

void GameState::inputListening()
{
	sf::TcpSocket& listeningSocket{_client.getGameListeningSocket()};
	_client.waitTillReadyToPlay();
	sf::Packet receivedPacket;
	TransferType type;
	sf::SocketSelector selector;
	selector.add(listeningSocket);
	while(_playing.load())
	{
		if(!selector.wait(SOCKET_TIME_SLEEP))
			continue;
		assert(selector.isReady(listeningSocket));
		auto receiveStatus{listeningSocket.receive(receivedPacket)};
		if(receiveStatus == sf::Socket::Disconnected)
		{
			std::cerr << "Connection lost with the server\n";
			_playing.store(false);
		}
		else if(receiveStatus == sf::Socket::Error)
		{
			std::cerr << "Error while transmitting data\n";
			_playing.store(false);
		}
		receivedPacket >> type;
		if(type == TransferType::GAME_OVER)
			_playing.store(false);
		else if(type == TransferType::GAME_PLAYER_ENTER_TURN)
			_myTurn.store(true);
		else if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
			_myTurn.store(false);
		else if(type == TransferType::GAME_PLAYER_ENERGY_UPDATED)
		{
			std::cout << "Energy points updated" << std::endl;
			// energy (and health and others) are transmitted through the network as 32 bit
			// unsigned integers. So be sure to receive the exact same thing to avoid encoding
			// errors and then set it in the "real" attribute
			sf::Uint32 energy32;
			_accessEnergy.lock();
			receivedPacket >> energy32;
			_selfEnergy = energy32;
			_accessEnergy.unlock();
			displayGame();
		}
		else if(type == TransferType::GAME_PLAYER_HEALTH_UPDATED)
		{
			std::cout << "Health points updated" << std::endl;
			sf::Uint32 health32;
			_accessHealth.lock();
			receivedPacket >> health32;
			_selfHealth = health32;
			_accessHealth.unlock();
			displayGame();
		}
		else if(type == TransferType::GAME_BOARD_UPDATED)
		{
			std::cout << "Board updated" << std::endl;
			receivedPacket >> _selfBoardCreatures;
			displayGame();
		}
		else if(type == TransferType::GAME_OPPONENT_BOARD_UPDATED)
		{
			std::cout << "Opponent's board updated" << std::endl;
			receivedPacket >> _oppoBoardCreatures;
			displayGame();
		}
		else if(type == TransferType::GAME_GRAVEYARD_UPDATED)
		{
			std::cout << "Graveyard updated" << std::endl;
			receivedPacket >> _selfGraveCards;
			displayGame();
		}
		else if(type == TransferType::GAME_HAND_UPDATED)
		{
			std::cout << "Hand updated" << std::endl;
			receivedPacket >> _selfHandCards;
			displayGame();
		}
		else
			std::cerr << "Unknown message received: " << static_cast<sf::Uint32>(type) << "; ignore." << std::endl;
	}
	std::cout << "GAME OVER\n";
}

//Not needed ?
/*
void GameState::updateData(std::array<unsigned, 5> data)
{
	_selfEnergy = data[0];
	_selfHealth = data[1]; _oppoHealth = data[2];
	_selfDeckSize = data[3];
	_oppoHandSize = data[4];

}

void GameState::changeHealth(int health)
{
	_selfHealth += health;
}

void GameState::changeOppoHealth(int health)
{
	_oppoHealth += health;
}

void GameState::changeEnergy(int energy)
{
	_selfEnergy += energy;
}*/
/*void GameState::pickCard(int pickedCard)
{
	if(_remainCards > 0)
	{
		_inHand.push_back(pickedCard);
		--_remainCards; // Player took a card from his deck
	}
}*/

/*
void GameState::putOnBoard(std::size_t cardIndex)
{
	int card = _inHand[cardIndex];  // Save the card ID
	_inHand.erase(_inHand.begin()+cardIndex);  // Remove it from the hand
	_onBoard.push_back(card);  // Put it on the board
}*/
/*
void GameState::applyOppoEffect()
{
	if(not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::size_t oppoCardIndex;
		std::cout << "On which opponent's creature would you like to apply the effect?\n";
		oppoCardIndex = selectOppo();

		if(oppoCardIndex > 0)
		{
			//TODO send the effect
			//Note that the indexes are vector's indexes +1
			//0 indicate there's no card in the vector.
		}
	}
}*/
/*
void GameState::applySelfEffect()
{
	if(not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		size_t selfCardIndex;
		std::cout << "On which of your creature would you like to apply the effect?\n";
		selfCardIndex = selectBoard();

		if (selfCardIndex > 0)
		{
			//TODO send the effect
			//Note that the indexes are vector's indexes +1
			//0 indicate there's no card in the vector.
		}
	}
}*/

std::string GameState::getCardName(cardId id)
{
	if (id<10)
		return ALL_CREATURES[id].name;
	else
		return ALL_SPELLS[id-10].name;
}

CostValue GameState::getCardCost(cardId id)
{
	if (id<10)
		return ALL_CREATURES[id].cost;
	else
		return ALL_SPELLS[id-10].cost;
}

std::string GameState::getCardDescription(cardId id)
{
	if (id<10)
		return ALL_CREATURES[id].description;
	else
		return ALL_SPELLS[id-10].description;
}
