#ifndef _GAME_THREAD_HPP_
#define _GAME_THREAD_HPP_

// std-C++ headers
#include <thread>
#include <atomic>
// WizardPoker headers
#include "server/Board.hpp"
#include "server/ClientInformations.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class GameThread final : public std::thread
{
public:
	/// Constructor
	GameThread(Player::ID player1ID, Player::ID player2ID);

	/// Constructor starting a thread
	template <class F, class ...Args>
	explicit GameThread(Player::ID player1ID, Player::ID player2ID, F&& f, Args&&... args);

	/// Functions which stops the running thread (abortion)
	void interruptGame();

	void establishSockets(const ClientInformations& player1, const ClientInformations& player2);

	/// Destructor
	~GameThread() = default;

	const Player::ID _player1ID;
	const Player::ID _player2ID;

private:
	//////////// Attributes
	std::atomic_bool _running;
	sf::TcpSocket _socketPlayer1;
	sf::TcpSocket _socketPlayer2;
	Board _gameBoard;

	//////////// Private methods
	void setSocket(sf::TcpSocket& socket, const ClientInformations& player);
};

///////// template code

template <typename Function, class... Args>
GameThread::GameThread(Player::ID player1ID, Player::ID player2ID, Function&& function, Args&&... args):
	std::thread(function, args...),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(true),
	_gameBoard(player1ID, player2ID, _socketPlayer1, _socketPlayer2)
{

}

#endif  // _GAME_THREAD_HPP_
