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
		throw std::runtime_error("Wrong signal received: " + std::to_string(static_cast<sf::Uint32>(type)));
	packet >> type;
	if(type == TransferType::GAME_PLAYER_ENTER_TURN)
		startTurn();
	else if(type == TransferType::GAME_PLAYER_LEAVE_TURN)
		_myTurn = false;
	else
		throw std::runtime_error("Wrong turn information: " + std::to_string(static_cast<sf::Uint32>(type)));

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
	std::cout << "It is now your turn";
	//TODO put the card taken in the _inHand vector;
	--_remainCards;  // Player took a card from his deck
	++_nbrTurn;
	setEnergy(_nbrTurn);
	display();
	/**/
}

void GameState::changeEnergy(unsigned energy)
{
	if(_energy+energy >= MAX_ENERGY)
		_energy = MAX_ENERGY;
	else
		_energy+=energy;
}


//PRIVATE METHODS

void GameState::setEnergy(unsigned energy)
{
	if(energy >= MAX_ENERGY)
		_energy = MAX_ENERGY;
	else
		_energy = energy;
}

void GameState::useCard()
{
	if (not _myTurn)
	{
		std::cout << "You must wait your turn!\n";
		return;
	}
	else
	{
		if(!_inHand.empty())
		{
			std::size_t cardIndex;
			std::cout << "Which card would you like to use?\n";
			for(std::size_t i = 0; i < _inHand.size(); ++i)
				std::cout << i << ". " << _inHand[i] << "\n";
			std::cout << "Enter the index of the card : ";
			std::cin >> cardIndex;
			putOnBoard(cardIndex);
		}
		else
			std::cout << "You have no card on the board";
	}
}

void GameState::putOnBoard(std::size_t cardIndex)
{
	int card = _inHand[cardIndex];			 ///Save the card ID
	_inHand.erase(_inHand.begin()+cardIndex);///Remove it from the hand
	_onBoard.push_back(card);				 ///Put it on the board
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
		std::size_t selfCardIndex, oppoCardIndex;
		std::cout << "With which creature would like to attack?\n";
		for(std::size_t i = 0; i < _onBoard.size(); ++i)
			std::cout << i << ". " << _onBoard[i] << "\n";
		std::cout << "Enter the index of the card : ";
		std::cin >> selfCardIndex;
		
		std::cout << "Which opponent's creature would like to attack?\n";
		for(std::size_t i = 0; i < _oppoBoard.size(); ++i)
			std::cout << i << ". " << _oppoBoard[i] << "\n";
		std::cout << "Enter the index of the card : ";
		std::cin >> oppoCardIndex;
		
		//TODO send the attack
	};
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
	};
}

void GameState::quit()
{
	stackPop();
}
