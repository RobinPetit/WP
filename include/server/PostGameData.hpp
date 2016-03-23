#ifndef _POST_GAME_DATA_SERVER_HPP_
#define _POST_GAME_DATA_SERVER_HPP_

// std-C++ headers
#include <vector>
#include <string>

struct PostGameData
{
	bool opponentInDaClub;
	bool playerStarted;
	bool playerTookDamage;
	bool playerWon;
	bool playerQuit;

	cardId unlockedCard;

	int remainingHealth;
	int gameDuration;
};

#endif  // _POST_GAME_DATA_SERVER_HPP_
