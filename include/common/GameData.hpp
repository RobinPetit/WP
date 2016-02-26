#ifndef _GAME_DATA_COMMON_HPP
#define _GAME_DATA_COMMON_HPP

#include <vector>
#include <string>
#include "common/Identifiers.hpp"

/// These struct are used to communicate changes of the game data on the network (server->client).
struct BoardCreatureData
{
	cardId id;
	int health;
	int attack;
	int shield;
	std::string shieldType;
	//bool isParalyzed;
};

#endif// _GAME_DATA_COMMON_HPP
