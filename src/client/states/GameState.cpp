// std-C++ headers
#include <iostream>
#include <cstdlib>
// SFML headers
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include "common/constants.hpp"
#include "client/ErrorCode.hpp"
#include "client/sockets/Client.hpp"
#include "common/sockets/TransferType.hpp"
#include "common/Terminal.hpp"
#include "common/ini/IniFile.hpp"
#include "client/states/GameState.hpp"

GameState::GameState(StateStack& stateStack, Client& client):
	AbstractState(stateStack, client)
{
	addAction("Quit", &GameState::quit);
	std::cout << "Your game is about to start!\n";
	_client.waitTillReadyToPlay();
	sf::Packet packet;
	_client.getGameSocket().receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::GAME_STARTING)
	{
		std::cerr << "Wrong signal received: " << static_cast<sf::Uint32>(type) << std::endl;
		quit();
	}
}

void GameState::display()
{
	std::cout << "Here are your options:\n";
	// Display the actions
	AbstractState::display();
}

void GameState::begin(unsigned lotsOfDataAboutStuff)
{
	_lotsOfDataAboutStuff = lotsOfDataAboutStuff;
	addAction("Use card from hand", &GameState::useCard);
	addAction("Attack with a creature", &GameState::attackWithCreature);
	addAction("End your turn", &GameState::endTurn);
	/**/
}

void GameState::startTurn()
{
	// if client dos NOT receive a message telling to start the turn,
	// there is an error -> throw std::runtime_error
	sf::Packet packet;
	_client.getGameSocket().receive(packet);
	TransferType type;
	packet >> type;
	if(type != TransferType::GAME_PLAYER_ENTER_TURN)
		throw std::runtime_error("Wrong message received");
	_myTurn = true;
	std::cout << "It is now your turn";
	/**/
}


//PRIVATE METHODS
void GameState::useCard()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	/**/
}

void GameState::attackWithCreature()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	/**/
}

void GameState::endTurn()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	/**/
}

void GameState::quit()
{
	stackPop();
}
