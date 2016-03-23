#ifndef _POST_GAME_DATA_SERVER_HPP_
#define _POST_GAME_DATA_SERVER_HPP_

// std-C++ headers
#include <vector>
#include <string>

struct PostGameData
{
	bool playerWon;
	bool opponentInDaClub;
	bool playerQuit;
	cardId unlockedCard;
    bool playerStarted;
    bool playerTookDamage;
    int remainingHealth;
    std::size_t gameDuration;
};

#endif  // _POST_GAME_DATA_SERVER_HPP_
