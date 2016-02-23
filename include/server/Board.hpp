#ifndef _BOARD_HPP
#define _BOARD_HPP

// WizardPoker headers
#include "server/Player.hpp"
#include "server/Card.hpp"
#include "server/Creature.hpp"
#include "server/Constraints.hpp"
#include "server/CardData.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

/// Manages one game with two players
class Board
{
public:
	/// Constructor
	Board(Player::ID player1, Player::ID player2, sf::TcpSocket& socketPlayer1, sf::TcpSocket& socketPlayer2);

	/// Destructor
	~Board() = default;

	/// Interface for user
	void endTurn();
	void useCard(int handIndex);
	void attackWithCreature(int boardIndex, int victim);
	void quitGame(); //TODO: need identifier

	Player::ID getCurrentPlayerID();
	Player::ID getWaitingPlayerID();

	/// Interface for Player and Card classes
	void applyEffect(Card* usedCard, EffectParamsCollection effect);

private:
	int _turn = 0;
	Player *_activePlayer, *_passivePlayer;
};


#endif// _BOARD_HPP
