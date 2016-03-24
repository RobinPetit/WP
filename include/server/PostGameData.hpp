#ifndef _POST_GAME_DATA_SERVER_HPP_
#define _POST_GAME_DATA_SERVER_HPP_

// std-C++ headers
#include <vector>
#include <string>

struct PostGameData
{
	///\TODO should be bool but I havent time to handle this for now
	int opponentInDaClub;
	int playerStarted;
	int playerTookDamage;
	int playerWon;
	int playerQuit;

	cardId unlockedCard;

	int remainingHealth;
	int gameDuration;
};

#endif  // _POST_GAME_DATA_SERVER_HPP_
