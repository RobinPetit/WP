#include "server/GameThread.hpp"

GameThread::GameThread(unsigned player1ID, unsigned player2ID):
	std::thread(),
	_running(false),
	_gameBoard(player1ID, player2ID)
{

}

void GameThread::interruptGame()
{
	_running.store(false);
}
