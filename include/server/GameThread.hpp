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
	GameThread(unsigned player1ID, unsigned player2ID);

	/// Constructor starting a thread
	template <class F, class ...Args>
	explicit GameThread(unsigned player1ID, unsigned player2ID, F&& f, Args&&... args);

	/// Functions which stops the running thread (abortion)
	void interruptGame();

	void establishSockets(const ClientInformations& player1, const ClientInformations& player2);

	/// Destructor
	~GameThread() = default;

	const unsigned _player1ID;
	const unsigned _player2ID;

private:
	std::atomic_bool _running;
	Board _gameBoard;
	sf::TcpSocket _socketPlayer1;
	sf::TcpSocket _socketPlayer2;

	void setSocket(sf::TcpSocket& socket, const ClientInformations& player);
};

template <typename Function, class... Args>
GameThread::GameThread(unsigned player1ID, unsigned player2ID, Function&& function, Args&&... args):
	std::thread(function, args...),
	_player1ID(player1ID),
	_player2ID(player2ID),
	_running(true),
	_gameBoard(player1ID, player2ID)
{

}

#endif  // _GAME_THREAD_HPP_
