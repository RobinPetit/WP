#ifndef _POST_GAME_DATA_SERVER_HPP_
#define _POST_GAME_DATA_SERVER_HPP_

// std-C++ headers
#include <string>
#include <cassert>
// WizardPoker headers
#include "common/Identifiers.hpp"

struct PostGameData
{
	///\TODO should be bool but I havent time to handle this for now
	int opponentInDaClub;
	int playerStarted;
	int playerTookDamage;
	int playerWon;
	int playerRageQuit; //meant for 'player broke computer instead'

	cardId unlockedCard;

	int remainingHealth;
	int gameDuration;
	PostGameData();
	std::string display();
};

#endif  // _POST_GAME_DATA_SERVER_HPP_
