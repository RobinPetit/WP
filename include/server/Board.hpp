#ifndef _BOARD_HPP
#define _BOARD_HPP

// WizardPoker headers
#include "server/Player.hpp"
#include "server/Card.hpp"
#include "server/Creature.hpp"
#include "server/Constraints.hpp"
#include "common/CardData.hpp"
#include "server/ServerDatabase.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

struct PlayerInformations
{
	userId id;
	sf::TcpSocket& socket;
	sf::TcpSocket& specialSocket;
};

/// Manages one game with two players
class Board
{
public:
	/// Constructor
	Board(ServerDatabase& database, const PlayerInformations& player1, const PlayerInformations& player2);

	/// Destructor
	~Board() = default;

	/// Interface for user
	void endTurn();
	void useCard(int handIndex);
	void attackWithCreature(int attackerIndex, int victimIndex);
	void quitGame(); //TODO: need identifier for player who quit

	userId getCurrentPlayerID();
	userId getWaitingPlayerID();

	/// Used by the GameThread instance when it gets the decks names from the users
	void givePlayersDecksNames(const std::string& player1DeckName, const std::string& player2DeckName);

	/// Interface for Player and Card classes
	void applyEffect(Card* usedCard, EffectParamsCollection effect);

private:
	ServerDatabase& _database;
	int _turn = 0;
	Player *_activePlayer, *_passivePlayer;
	bool turnCanEnd=false;
};

#endif// _BOARD_HPP
