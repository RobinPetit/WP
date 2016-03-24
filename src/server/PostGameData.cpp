#include "server/PostGameData.hpp"

PostGameData::PostGameData() :
	opponentInDaClub(false),
	playerStarted(false),
	playerTookDamage(false),
	playerWon(false),
	playerQuit(false),
	unlockedCard(0)
{
	assert((int) false == 0);
	assert((int) true == 1);
}

std::string PostGameData::display()
{
	return 	std::string("\n-opponentInDaClub : ") + std::to_string(opponentInDaClub) +
			std::string("\n-playerStarted : ") + std::to_string(playerStarted) +
			std::string("\n-playerTookDamage : ") + std::to_string(playerTookDamage) +
			std::string("\n-playerWon : ") + std::to_string(playerWon) +
			std::string("\n-playerQuit : ") + std::to_string(playerQuit) +
			std::string("\n-unlockedCard : ") + std::to_string(unlockedCard) +
			std::string("\n-remainingHealth : ") + std::to_string(remainingHealth) +
			std::string("\n-gameDuration : ") + std::to_string(gameDuration);
}
