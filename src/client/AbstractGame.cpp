// std-C++ headers
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <limits>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketSelector.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/sockets/PacketOverload.hpp"
#include "common/sockets/EndGame.hpp"
#include "client/NonBlockingInput.hpp"
#include "client/AbstractGame.hpp"

const std::vector<std::pair<const std::string, void (AbstractGame::*)()>> AbstractGame::_actions =
{
	{"Quit", &AbstractGame::quit},
	{"Use a card from hand", &AbstractGame::useCard},
	{"Attack with a creature", &AbstractGame::attackWithCreature},
	{"End your turn", &AbstractGame::endTurn},
};

AbstractGame::AbstractGame(Client& client):
	_client{client},
	_playing(false)
{
	_client.waitTillReadyToPlay();
	_playing.store(true);
}

void AbstractGame::init()
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
	{
		_myTurn.store(false);
		displayGame();
		displayMessage("Your opponent begins, wait for your turn...");
	}
	else
		throw std::runtime_error("Wrong turn information: " + std::to_string(static_cast<sf::Uint32>(type)));
}

void AbstractGame::play()
{
	while(_playing.load())
	{
		if(_myTurn.load())
			startTurn();
		else
		{
			while(not _myTurn.load())
				sf::sleep(sf::milliseconds(100));
		}
	}
	if(_listeningThread.joinable())
		_listeningThread.join();
}

void AbstractGame::startTurn()
{
	_myTurn.store(true);
	displayGame();
}


//PRIVATE METHODS

//User actions
void AbstractGame::useCard()
{
	if (not _myTurn)
	{
		displayMessage("You must wait your turn!");
		return;
	}
	else
	{
		int cardIndex = askSelfHandIndex();
		sf::Packet actionPacket;
		actionPacket << TransferType::GAME_USE_CARD << static_cast<sf::Int32>(cardIndex);
		_client.getGameSocket().send(actionPacket);
		// receive amount of selection to make
		_client.getGameSocket().receive(actionPacket);
		std::vector<CardToSelect> requiredInputs;
		actionPacket >> requiredInputs;
		// ask and send the additionnal inputs to the server
		treatAdditionnalInputs(requiredInputs);
		// receive status of operation from server
		_client.getGameSocket().receive(actionPacket);
		TransferType responseHeader;
		actionPacket >> responseHeader;
		switch(responseHeader)
		{
		case TransferType::ACKNOWLEDGE:
			displayMessage("The card has been successfuly used.");
			break;

		case TransferType::GAME_NOT_ENOUGH_ENERGY:
			displayMessage("You have not enough energy to use this card.");
			break;

		case TransferType::GAME_CARD_LIMIT_TURN_REACHED:
			displayMessage("You can't use more cards, the limit is reached.");
			break;

		// This line is more documentative than really needed
		case TransferType::FAILURE:
		default:
			displayMessage("An error occured when using a card.");
			break;
		}
	}
}

void AbstractGame::treatAdditionnalInputs(const std::vector<CardToSelect>& inputs)
{
	std::vector<sf::Uint32> indices;
	indices.reserve(inputs.size());
	for(const auto& input : inputs)
	{
		switch(input)
		{
		case CardToSelect::SELF_BOARD:
			indices.push_back(askSelfBoardIndex());
			break;

		case CardToSelect::OPPO_BOARD:
			indices.push_back(askOppoBoardIndex());
			break;

		case CardToSelect::SELF_HAND:
			indices.push_back(askSelfHandIndex());
			break;

		case CardToSelect::OPPO_HAND:
			indices.push_back(askOppoHandIndex());
			break;
		}
	}
	sf::Packet indicesPacket;
	indicesPacket << indices;
	_client.getGameSocket().send(indicesPacket);
}

void AbstractGame::attackWithCreature()
{
	if (not _myTurn)
	{
		displayMessage("You must wait your turn!");
		return;
	}
	else
	{
		displayMessage("Which creature would you like to attack with?");
		int selfCardIndex = askSelfBoardIndex();
		int oppoCardIndex;
		bool attackOpponent{wantToAttackOpponent()};
		if(not attackOpponent)
		{
			displayMessage("Which opponent's creature would you like to attack?");
			oppoCardIndex = askOppoBoardIndex();
		}

		// If there's cards on board
		if(selfCardIndex >= 0 and (oppoCardIndex >= 0 or attackOpponent))
		{
			if(attackOpponent)
				oppoCardIndex = -1;
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
				displayMessage("The monster successfuly attacked.");
				break;

			case TransferType::GAME_NOT_ENOUGH_ENERGY:
				displayMessage("You have not enough energy to attack with this monster.");
				break;

			// This line is more documentative than really needed
			case TransferType::FAILURE:
			default:
				displayMessage("An error occured when attacking with this monster.");
				break;
			}
		}
		else
			displayMessage("There is no card to choose, please do something else.");
	}
}


void AbstractGame::endTurn()
{
	if (not _myTurn)
	{
		displayMessage("You must wait your turn!\n");
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

void AbstractGame::quit()
{
	// send QUIT message to server
	sf::Packet actionPacket;
	actionPacket << TransferType::GAME_QUIT_GAME;
	_client.getGameSocket().send(actionPacket);
	// internal ending
	_playing.store(false);
	_myTurn.store(false);
}

AbstractGame::~AbstractGame()
{
	quit();
}

//////////////// special data receival

void AbstractGame::initListening()
{
	_listeningThread = std::thread(&AbstractGame::inputListening, this);
}

void AbstractGame::inputListening()
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
			handlePacket(receivedPacket);
			// do not re-display in game menu if game is over or if player is currently playing
			if(_playing.load() && not _myTurn.load())
				displayGame();
		}
	}
}

void AbstractGame::endGame(sf::Packet& transmission)
{
	EndGame endGameInfo;
	transmission >> endGameInfo;
	if(endGameInfo.applyToSelf)
	{
		switch(endGameInfo.cause)
		{
		case EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK:
			displayMessage("You lost because you played 10 turns with an empty deck!");
			break;

		case EndGame::Cause::OUT_OF_HEALTH:
			displayMessage("You lost because you ran out of health!");
			break;

		case EndGame::Cause::QUITTED:
			displayMessage("You quitted the game.");
			break;
		}
	}
	else
	{
		switch(endGameInfo.cause)
		{
		case EndGame::Cause::TEN_TURNS_WITH_EMPTY_DECK:
			displayMessage("You won because your opponent played 10 turns with an empty deck!");
			break;

		case EndGame::Cause::OUT_OF_HEALTH:
			displayMessage("You won because your opponent ran out of health!");
			break;

		case EndGame::Cause::QUITTED:
			displayMessage("You won because your opponent quitted the game!");
			break;
		}
		// Get the won card
		cardId newCard;
		transmission >> newCard;
		receiveCard(newCard);
	}
	_playing.store(false);
	_myTurn.store(!_myTurn.load());
}

void AbstractGame::handlePacket(sf::Packet& transmission)
{
	// TODO: std::couts here are debug. To remove
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
			displayMessage("You got the turn");
			_myTurn.store(true);
			break;

		case TransferType::GAME_PLAYER_LEAVE_TURN:
			displayMessage("You lost the turn");
			_myTurn.store(false);
			break;

		case TransferType::GAME_PLAYER_ENERGY_UPDATED:
		{
			// energy (and health and others) are transmitted through the network as 32 bit
			// unsigned integers. So be sure to receive the exact same thing to avoid encoding
			// errors and then set it in the "real" attribute
			std::lock_guard<std::mutex> lock{_accessEnergy};
			_selfEnergy = receiveFromPacket<sf::Uint32>(transmission);
		}
			break;

		case TransferType::GAME_PLAYER_HEALTH_UPDATED:
		{
			std::lock_guard<std::mutex> lock{_accessHealth};
			_selfHealth = receiveFromPacket<sf::Uint32>(transmission);
		}
			break;

		case TransferType::GAME_OPPONENT_HEALTH_UPDATED:
		{
			std::lock_guard<std::mutex> lock{_accessHealth};
			_oppoHealth = receiveFromPacket<sf::Uint32>(transmission);
		}
			break;

		case TransferType::GAME_BOARD_UPDATED:
			_selfBoardCreatures.clear();
			transmission >> _selfBoardCreatures;
			break;

		case TransferType::GAME_OPPONENT_BOARD_UPDATED:
			_oppoBoardCreatures.clear();
			transmission >> _oppoBoardCreatures;
			break;
		case TransferType::GAME_GRAVEYARD_UPDATED:
			_selfGraveCards.clear();
			transmission >> _selfGraveCards;
			break;

		case TransferType::GAME_HAND_UPDATED:
			_selfHandCards.clear();
			transmission >> _selfHandCards;
			break;

		case TransferType::GAME_OPPONENT_HAND_UPDATED:
			_oppoHandSize = receiveFromPacket<sf::Uint32>(transmission);
			break;

		case TransferType::GAME_DECK_UPDATED:
			_selfDeckSize = receiveFromPacket<sf::Uint32>(transmission);
			break;

		default:
			std::cerr << "Unknown message received: " << static_cast<std::underlying_type<TransferType>::type>(type) << "; ignore." << std::endl;
			break;
		}
	}
}

const std::string& AbstractGame::getCardName(cardId id)
{
	const CommonCardData* card = _client.getCardData(id);
	// Maybe I should have use multiple inheritance to avoid this
	return card->isSpell()
		? static_cast<const ClientSpellData*>(card)->getName()
		: static_cast<const ClientCreatureData*>(card)->getName();
}

CostValue AbstractGame::getCardCost(cardId id)
{
	const CommonCardData* card = _client.getCardData(id);
	// Maybe I should have use multiple inheritance to avoid this
	return card->isSpell()
		? static_cast<CostValue>(static_cast<const ClientSpellData*>(card)->getCost())
		: static_cast<CostValue>(static_cast<const ClientCreatureData*>(card)->getCost());
}

const std::string& AbstractGame::getCardDescription(cardId id)
{
	const CommonCardData* card = _client.getCardData(id);
	// Maybe I should have use multiple inheritance to avoid this
	return card->isSpell()
		? static_cast<const ClientSpellData*>(card)->getDescription()
		: static_cast<const ClientCreatureData*>(card)->getDescription();
}

bool AbstractGame::isSpell(cardId id)
{
	return _client.getCardData(id)->isSpell();
}

template <typename FixedSizeIntegerType>
FixedSizeIntegerType AbstractGame::receiveFromPacket(sf::Packet& receivedPacket)
{
	assert(not receivedPacket.endOfPacket());
	FixedSizeIntegerType value;
	receivedPacket >> value;
	return std::move(value);
}
