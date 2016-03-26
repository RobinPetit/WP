#ifndef _GAME_THREAD_HPP_
#define _GAME_THREAD_HPP_

// std-C++ headers
#include <thread>
#include <atomic>
// WizardPoker headers
#include "server/Player.hpp"
#include "server/ClientInformations.hpp"
#include "common/Identifiers.hpp"  // UserId
#include "server/ServerDatabase.hpp"
#include "common/sockets/EndGame.hpp"
#include "common/random/RandomInteger.hpp"
#include "server/PostGameData.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class GameThread final : public std::thread
{
public:
	/*------------------------------ Attributes */
	const UserId _player1Id;
	const UserId _player2Id;

	/*------------------------------ Methods */
	/// Constructor
	GameThread(ServerDatabase& database, UserId player1Id, UserId player2Id);

	/// Constructor starting a thread
	template <class F, class ...Args>
	explicit GameThread(ServerDatabase& database, UserId player1Id, UserId player2Id, F&& f, Args&&... args);

	/// Interface for Server

	/// Creates the in-game sockets (normal and special) to the players
	/// \param player 1 \see playGame for more informations
	void establishSockets(const ClientInformations& player1, const ClientInformations& player2);

	/// starts and plays a game. Returns only when one of the players lost (or when the
	/// server is asked to end)
	/// \param player1 The data (port/address) for the first player
	/// \param player2 \see player1
	/// \return The id of the winner
	UserId playGame(const ClientInformations& player1, const ClientInformations& player2);
	void interruptGame(); ///< Stops the running thread (abort)

	/// Interface for Player

	/// Signals the game is over and register the winner and the reason of the win
	/// \param winnerId The id of the winner
	/// \param cause The reason of the end of the game
	void endGame(UserId winnerId, EndGame::Cause cause);

	/// Method to call to force the end of the current turn and the start of the other player's turn
	void swapTurns();

	/// Gives the random generator
	RandomInteger& getGenerator();

	/// Debug method printing \a message
	void printVerbose(const std::string& message);

	/// Destructor
	~GameThread();

private:
	/*------------------------------ Attributes */
	std::atomic_bool _running;

	Player _player1;
	Player _player2;
	Player* _activePlayer;
	Player* _passivePlayer;

	sf::TcpSocket _specialOutputSocketPlayer1;
	sf::TcpSocket _specialOutputSocketPlayer2;
	sf::TcpSocket* _activeSpecialSocket;
	sf::TcpSocket* _passiveSpecialSocket;

	PostGameData _postGameDataPlayer1;
	PostGameData _postGameDataPlayer2;
	ServerDatabase& _database;

	UserId _winnerId;
	EndGame::Cause _endGameCause;

	int _turn;
	bool _verbose=true;

	std::thread _timerThread;
	std::atomic_bool _turnSwap;
	std::chrono::high_resolution_clock::time_point _startOfTurnTime;

	RandomInteger _intGenerator;

	/*------------------------------ Static variables */
	/// Currently low for tests, arbitrary, need more time now for testing
	static constexpr std::chrono::seconds _turnTime{120};  // TODO: change this

	/*------------------------------ Methods */
	void createPlayers();

	/// Main loop of the game: waits for each side inputs and forces turns swapping
	void runGame();

	void setSocket(sf::TcpSocket& socket, sf::TcpSocket& specialSocket, const ClientInformations& player);

	void makeTimer();

	void endTurn();
	void swapData();

	void sendFinalMessage(sf::TcpSocket& specialSocket, PostGameData& postGameData, CardId earnedCardId, AchievementList& newAchievements);
};

/*------------------------------ Template code */

template <typename Function, class... Args>
GameThread::GameThread(ServerDatabase& database, UserId player1Id, UserId player2Id, Function&& function, Args&&... args):
	std::thread(function, args...),
	_player1Id(player1Id),
	_player2Id(player2Id),
	_running(true),
	_player1(*this, database, _player1Id, _player2, _postGameDataPlayer1),
	_player2(*this, database, _player2Id, _player1, _postGameDataPlayer2),
	_database(database),
	_winnerId{0},
	_turn(0),
	_turnSwap{false}
{
	createPlayers();
}

#endif  // _GAME_THREAD_HPP_
