// std-C++ headers
#include <iostream>
#include <cstdlib>
#include <cassert>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "client/Game.hpp"
#include "client/NonBlockingInput.hpp"

const std::vector<std::pair<const std::string, void (Game::*)()>> Game::_actions =
{
	{"Quit", &Game::quit},
	{"Use a card from hand", &Game::useCard},
	{"Attack with a creature", &Game::attackWithCreature},
	{"End your turn", &Game::endTurn},
};

Game::Game(Client& client):
	_client{client},
	_playing(false)
{
	std::cout << "Your game is about to start!\n";
	_client.waitTillReadyToPlay();
	_playing.store(true);
	init();
}

void Game::init()
{
	initListening();
	chooseDeck();
	sf::Packet packet;

	// receive in game data
	_client.getGameSocket().receive(packet);
	handlePacket(packet);

	// receive turn informations
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

void Game::chooseDeck()
{
	std::vector<Deck> decks = _client.getDecks();

	// Ask for the deck to use during the game
	for(std::size_t i{0}; i < decks.size(); i++)
		std::cout << i << " : " << decks.at(i).getName() << std::endl;
	std::size_t res;  // Chosen deck
	do
	{
		std::cout << "Choose your deck: ";
		std::cin >> res;
		if(res >= decks.size())
			std::cout << "Your answer should be in the range [" << 0 << ", " << decks.size() <<"[ !\n";
	} while(res >= decks.size());

	// Send the deck name to the server
	std::cout << "sending deck " << decks.at(res).getName() << std::endl;
	sf::Packet deckNamePacket;
	deckNamePacket << TransferType::GAME_PLAYER_GIVE_DECK_NAMES << decks.at(res).getName();
	_client.getGameSocket().send(deckNamePacket);
}

void Game::play()
{
	while(_playing.load())
	{
		if(_myTurn.load())
			startTurn();
		else
		{
			std::cout << "Waiting for your turn...\n";
			while(not _myTurn.load())
				sf::sleep(sf::milliseconds(100));
		}
	}
	_listeningThread.join();
}

void Game::display()
{
	displayGame();
	std::cout << "Here are your options:\n";
	std::size_t idx{0};
	for(const auto& action: _actions)
		std::cout << idx++ << ". " << action.first << std::endl;
}

void Game::startTurn()
{
	NonBlockingInput input;
	_myTurn.store(true);
	display();
	std::cout << "It is now your turn, what do you want to do? ";
	while(_myTurn.load())
	{
		if(not input.waitForData(0.1))
			continue;
		std::string command{input.receiveStdinData()};
		try
		{
			(this->*(_actions.at(std::stoi(command)).second))();
		}
		catch(std::exception&)
		{
			std::cout << "Your input should be in [" << 0 << ", " << _actions.size() << ")\n";
			continue;
		}
		display();
		std::cout << "What do you want to do next? ";
	}
}


//PRIVATE METHODS

// Request user for additional input
int Game::askIndex(std::size_t upperBound, std::string inputMessage)
{
	int res;
	if (upperBound == 0)
	{
		std::cout << "There are no cards to choose.\n";
		return -1;
	}

	bool firstTry{true};
	do
	{
		if(not firstTry)
			std::cout << "Your answer should be in the range [" << 0 << ", " << upperBound <<"[ !\n";
		std::cout << inputMessage;
		std::cin >> res;
		firstTry = false;
	} while(res >= static_cast<int>(upperBound) or res < 0);

	return res;
}

int Game::askSelfHandIndex()
{
	std::cout << "These are the cards in your hand:" << std::endl;
	displayCardVector(_selfHandCards);
	return askIndex(_selfHandCards.size(), "Choose the index for a card in your hand: ");
}

int Game::askSelfBoardIndex()
{
	std::cout << "These are the cards on your board:" << std::endl;
	displayBoardCreatureVector(_selfBoardCreatures);
	return askIndex(_selfBoardCreatures.size(), "Choose the index for a card on your board: ");
}

int Game::askSelfGraveyardIndex()
{
	std::cout << "These are the cards in your graveyard:" << std::endl;
	displayCardVector(_selfGraveCards);
	return askIndex(_selfGraveCards.size(), "Choose the index for a card in the graveyard: ");
}

int Game::askOppoHandIndex()
{
	std::cout << "Your opponent has " << _oppoHandSize << " cards in his hand." << std::endl;
	return askIndex(_oppoHandSize, "Choose the index for a card in the opponent's hand: ");
}

int Game::askOppoBoardIndex()
{
	std::cout << "These are the cards on your opponent's board:" << std::endl;
	displayBoardCreatureVector(_oppoBoardCreatures);
	return askIndex(_oppoBoardCreatures.size(), "Choose the index for a card on the opponent's board: ");
}

//User actions
void Game::useCard()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		int cardIndex = askSelfHandIndex();
		sf::Packet actionPacket;
		actionPacket << TransferType::GAME_USE_CARD << static_cast<sf::Int32>(cardIndex);
		_client.getGameSocket().send(actionPacket);
		_client.getGameSocket().receive(actionPacket);
		TransferType responseHeader;
		actionPacket >> responseHeader;
		switch(responseHeader)
		{
		case TransferType::ACKNOWLEDGE:
			std::cout << "The card has been successfuly used.\n";
			break;

		case TransferType::GAME_NOT_ENOUGH_ENERGY:
			std::cout << "You have not enough energy to use this card.\n";
			break;

		case TransferType::GAME_CARD_LIMIT_TURN_REACHED:
			std::cout << "You can't use more cards, the limit is reached.\n";
			break;

		// This line is more documentative than really needed
		case TransferType::FAILURE:
		default:
			std::cout << "An error occured when using a card.\n";
			break;
		}
	}
}

void Game::attackWithCreature()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		std::cout << "Which creature would you like to attack with? \n";
		int selfCardIndex = askSelfBoardIndex();

		std::cout << "Which opponent's creature would you like to attack? \n";
		int oppoCardIndex = askOppoBoardIndex();

		// If there's cards on board
		if(selfCardIndex >= 0 && oppoCardIndex >= 0)
		{
			sf::Packet actionPacket;
			actionPacket << TransferType::GAME_ATTACK_WITH_CREATURE
			             << static_cast<sf::Int32>(selfCardIndex)
			             << static_cast<sf::Int32>(oppoCardIndex);
			_client.getGameSocket().send(actionPacket);
			_client.getGameSocket().receive(actionPacket);
			TransferType responseHeader;
			actionPacket >> responseHeader;
			switch(responseHeader)
			{
			case TransferType::ACKNOWLEDGE:
				std::cout << "The monster successfuly attacked.\n";
				break;

			case TransferType::GAME_NOT_ENOUGH_ENERGY:
				std::cout << "You have not enough energy to attack with this monster.\n";
				break;

			// This line is more documentative than really needed
			case TransferType::FAILURE:
			default:
				std::cout << "An error occured when attacking with this monster.\n";
				break;
			}
		}
		else
			std::cout << "There is no card to choose, please do something else.\n";
	}
}


void Game::endTurn()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		sf::Packet actionPacket;
		actionPacket << TransferType::GAME_PLAYER_LEAVE_TURN;
		_client.getGameSocket().send(actionPacket);
		_myTurn = false;
	}
}

void Game::quit()
{
	// send QUIT message to server
	sf::Packet actionPacket;
	actionPacket << TransferType::GAME_QUIT_GAME;
	_client.getGameSocket().send(actionPacket);
	// internal ending
	_playing.store(false);
	_myTurn.store(false);
}

Game::~Game()
{
	quit();
}

void Game::displayGame()
{
	_client.getTerminal().clearScreen();
	std::cout << "***************" << std::endl;
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
	std::cout << "***************" << std::endl;
}

void Game::displayCardVector(const std::vector<CardData>& cardVector)
{
	for (auto i=0U; i<cardVector.size(); i++)
	{
		cardId id = cardVector.at(i).id;
		std::cout << "  * " << i << " : " << getCardName(id) << " (cost: " << getCardCost(id)
		          << ")" << (i < cardVector.size()-1 ? ", " : "") << "\n";
	}
}

void Game::displayBoardCreatureVector(const std::vector<BoardCreatureData>& cardVector)
{
	// The board vector also contains real time informations about the cards (health, attack, shield, shield type)
	// This method should display these informations and be called only for displaying the board
	for (auto i=0U; i<cardVector.size(); i++)
	{
		const BoardCreatureData& thisCreature = cardVector.at(i);
		const cardId id = thisCreature.id;
		std::cout << i << " : " << getCardName(id) << " (cost: " << getCardCost(id) <<
		             ", attack: " << thisCreature.attack <<
		             ", health: " << thisCreature.health <<
		             ", shield: " << thisCreature.shield << "-" << thisCreature.shieldType << ")";
		//TODO use card names instead of card IDs ?
		if (i!=cardVector.size()-1)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

//////////////// special data receival

void Game::initListening()
{
	_listeningThread = std::thread(&Game::inputListening, this);
}

void Game::inputListening()
{
	sf::TcpSocket& listeningSocket{_client.getGameListeningSocket()};
	_client.waitTillReadyToPlay();
	sf::Packet receivedPacket;
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
		else
		{
			std::cout << "handling packet\n";
			handlePacket(receivedPacket);
			// do not re-display in game menu if game is over
			if(_playing.load())
				displayGame();
		}
	}
}

void Game::endGame(sf::Packet& transmission)
{
	EndGame endGameInfo;
	transmission >> endGameInfo;
	if(endGameInfo.applyToSelf)
	{
		switch(endGameInfo.cause)
		{
		case EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK:
			std::cout << "You lost because you played 10 turns with an empty deck!\n";
			break;

		case EndGame::Cause::OUT_OF_HEALTH:
			std::cout << "You lost because you ran out of health!\n";
			break;

		case EndGame::Cause::QUITTED:
			std::cout << "You quitted the game.\n";
			break;
		}
	}
	else
	{
		switch(endGameInfo.cause)
		{
		case EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK:
			std::cout << "You won because your opponent played 10 turns with an empty deck!\n";
			break;

		case EndGame::Cause::OUT_OF_HEALTH:
			std::cout << "You won because your opponent ran out of health!\n";
			break;

		case EndGame::Cause::QUITTED:
			std::cout << "You won because your opponent quitted the game!\n";
			break;
		}
		// Get the won card
		cardId newCard;
		transmission >> newCard;
		std::cout << "You won the card '" << getCardName(newCard) << "'\n";
	}
	_playing.store(false);
	_myTurn.store(!_myTurn.load());
}

void Game::handlePacket(sf::Packet& transmission)
{
	TransferType type;
	while(not transmission.endOfPacket())
	{
		transmission >> type;
		switch(type)
		{
		case TransferType::GAME_OVER:
			endGame(transmission);
			break;

		case TransferType::GAME_PLAYER_ENTER_TURN:
			_myTurn.store(true);
			break;

		case TransferType::GAME_PLAYER_LEAVE_TURN:
			_myTurn.store(false);
			break;

		case TransferType::GAME_PLAYER_ENERGY_UPDATED:
		{
			// energy (and health and others) are transmitted through the network as 32 bit
			// unsigned integers. So be sure to receive the exact same thing to avoid encoding
			// errors and then set it in the "real" attribute
			sf::Uint32 energy32;
			transmission >> energy32;
			std::lock_guard<std::mutex> lock{_accessEnergy};
			_selfEnergy = energy32;
		}
			break;

		case TransferType::GAME_PLAYER_HEALTH_UPDATED:
		{
			sf::Uint32 health32;
			transmission >> health32;
			std::lock_guard<std::mutex> lock{_accessHealth};
			_selfHealth = health32;
		}
			break;

		case TransferType::GAME_OPPONENT_HEALTH_UPDATED:
		{
			sf::Uint32 health32;
			transmission >> health32;
			std::lock_guard<std::mutex> lock{_accessHealth};
			_oppoHealth = health32;
		}
			break;

		case TransferType::GAME_BOARD_UPDATED:
			std::cout << "Board updated" << std::endl;
			_selfBoardCreatures.clear();
			transmission >> _selfBoardCreatures;
			break;

		case TransferType::GAME_OPPONENT_BOARD_UPDATED:
			std::cout << "Opponent's board updated" << std::endl;
			_oppoBoardCreatures.clear();
			transmission >> _oppoBoardCreatures;
			break;
		case TransferType::GAME_GRAVEYARD_UPDATED:
			std::cout << "Graveyard updated" << std::endl;
			_selfGraveCards.clear();
			transmission >> _selfGraveCards;
			break;

		case TransferType::GAME_HAND_UPDATED:
			std::cout << "Hand updated" << std::endl;
			_selfHandCards.clear();
			transmission >> _selfHandCards;
			break;

		default:
			std::cerr << "Unknown message received: " << static_cast<std::underlying_type<TransferType>::type>(type) << "; ignore." << std::endl;
			break;
		}
	}
}

/*void Game::pickCard(int pickedCard)
{
	if(_remainCards > 0)
	{
		_inHand.push_back(pickedCard);
		--_remainCards; // Player took a card from his deck
	}
}*/

/*
void Game::putOnBoard(std::size_t cardIndex)
{
	int card = _inHand[cardIndex];  // Save the card ID
	_inHand.erase(_inHand.begin()+cardIndex);  // Remove it from the hand
	_onBoard.push_back(card);  // Put it on the board
}*/
/*
void Game::applyOppoEffect()
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
void Game::applySelfEffect()
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

const std::string& Game::getCardName(cardId id)
{
	if (id<=10)
		return ALL_CREATURES[id-1].name;
	else
		return ALL_SPELLS[id-11].name;
}

CostValue Game::getCardCost(cardId id)
{
	if (id<=10)
		return ALL_CREATURES[id-1].cost;
	else
		return ALL_SPELLS[id-11].cost;
}

const std::string& Game::getCardDescription(cardId id)
{
	if (id<=10)
		return ALL_CREATURES[id-1].description;
	else
		return ALL_SPELLS[id-11].description;
}
