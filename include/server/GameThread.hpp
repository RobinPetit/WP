#ifndef _GAME_THREAD_HPP_
#define _GAME_THREAD_HPP_

// std-C++ headers
#include <thread>
#include <atomic>
// WizardPoker headers
#include "server/Player.hpp"
#include "server/ClientInformations.hpp"
#include "common/Identifiers.hpp"  // userId
#include "server/ServerDatabase.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class GameThread final : public std::thread
{
public:
	/// Constructor
	GameThread(ServerDatabase& database, userId player1ID, userId player2ID);

	/// Constructor starting a thread
	template <class F, class ...Args>
	explicit GameThread(ServerDatabase& database, userId player1ID, userId player2ID, F&& f, Args&&... args);

	/// Functions which stops the running thread (abortion)
	void interruptGame();

	void establishSockets(const ClientInformations& player1, const ClientInformations& player2);

	userId startGame(const ClientInformations& player1, const ClientInformations& player2);

	void endGame(userId winnerId, EndGame::Cause cause);

	void swapTurns();

	/// Destructor
	~GameThread();

	// Public attributes
	const userId _player1ID;
	const userId _player2ID;

private:
	//////////// Attributes
	std::atomic_bool _running;
	sf::TcpSocket _specialOutputSocketPlayer1;
	sf::TcpSocket _specialOutputSocketPlayer2;
	Player _player1;
	Player _player2;
	ServerDatabase& _database;

	userId _winner;
	EndGame::Cause _endGamecause;

	Player *_activePlayer;
	Player *_passivePlayer;
	int _turn;
	bool _turnCanEnd;
	std::thread _timerThread;
	std::atomic_bool _turnSwap;
	// Currently low for tests
	static constexpr std::chrono::seconds _turnTime{30};  // arbitrary

	//////////// Private methods
	userId runGame();

	void setSocket(sf::TcpSocket& socket, sf::TcpSocket& specialSocket, const ClientInformations& player);

	void makeTimer();

	void createPlayers();
	void endTurn();

	void useCard(int cardIndex);
	void attackWithCreature(int attackerIndex, int victimIndex);
	void applyEffect(Card* usedCard, EffectParamsCollection effectArgs);
};

///////// template code

template <typename Function, class... Args>
GameThread::GameThread(ServerDatabase& database, userId player1ID, userId player2ID, Function&& function, Args&&... args):
	std::thread(function, args...),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(true),
	_player1(*this, database, _player1ID),
	_player2(*this, database, _player2ID),
	_database(database),
	_winner{0},
	_turn(0),
	_turnCanEnd(false),
	_turnSwap{false}
{
	createPlayers();
}

#endif  // _GAME_THREAD_HPP_
