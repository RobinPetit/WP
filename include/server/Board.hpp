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

struct PlayerInformations
{
	Player::ID id;
	sf::TcpSocket& socket;
	sf::TcpSocket& specialSocket;
};

/// Manages one game with two players
class Board
{
public:
	/// Constructor
	Board(const PlayerInformations& player1, const PlayerInformations& player2);

	/// Destructor
	~Board() = default;

	/// Interface for user
	void endTurn();
	void useCard(int handIndex);
	void attackWithCreature(int attackerIndex, int victimIndex);
	void quitGame(); //TODO: need identifier for player who quit

	Player::ID getCurrentPlayerID();
	Player::ID getWaitingPlayerID();

	/// Interface for Player and Card classes
	void applyEffect(Card* usedCard, EffectParamsCollection effect);

private:
	int _turn = 0;
	Player *_activePlayer, *_passivePlayer;
};


#endif// _BOARD_HPP
