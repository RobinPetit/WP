#ifndef _GAME_THREAD_HPP_
#define _GAME_THREAD_HPP_

// std-C++ headers
#include <thread>
#include <atomic>
// WizardPoker headers
#include "server/Board.hpp"
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

	/// \TODO change return value to give the result of the game
	void startGame(const ClientInformations& player1, const ClientInformations& player2);

	/// Destructor
	~GameThread();

	const userId _player1ID;
	const userId _player2ID;

	enum class PlayerNumber { PLAYER1, PLAYER2 };

	//~Currently low for tests
	static constexpr std::chrono::seconds _turnTime{30};  // arbitrary

private:
	//////////// Attributes
	std::atomic_bool _running;
	sf::TcpSocket _socketPlayer1;
	sf::TcpSocket _socketPlayer2;
	sf::TcpSocket _specialOutputSocketPlayer1;
	sf::TcpSocket _specialOutputSocketPlayer2;
	Board _gameBoard;
	// The Board class has one chance over two to swap the player (to randomize the first player).
	// This boolean is set to true if GameThread::player1 is Board::player1 and to false if GameThread::player1 is Board::player2.
	bool _isSynchroWithBoard;

	std::thread _timerThread;

	std::atomic_bool _turnSwap;

	std::string _player1DeckName;
	std::string _player2DeckName;

	//////////// Private methods
	void setSocket(sf::TcpSocket& socket, sf::TcpSocket& specialSocket, const ClientInformations& player);

	PlayerNumber PlayerFromID(const userId ID);
	sf::TcpSocket& getSocketFromID(const userId ID);
	sf::TcpSocket& getSpecialSocketFromID(const userId ID);

	void makeTimer();
	void receiveDecks();
};

///////// template code

template <typename Function, class... Args>
GameThread::GameThread(ServerDatabase& database, userId player1ID, userId player2ID, Function&& function, Args&&... args):
	std::thread(function, args...),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(true),
	_gameBoard(database, {player1ID, _socketPlayer1, _specialOutputSocketPlayer1}, {player2ID, _socketPlayer2, _specialOutputSocketPlayer2}),
	_isSynchroWithBoard(_player1ID == _gameBoard.getCurrentPlayerID())
{

}

#endif  // _GAME_THREAD_HPP_
