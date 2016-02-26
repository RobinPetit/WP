#ifndef _GAME_DATA_COMMON_HPP
#define _GAME_DATA_COMMON_HPP

#include "common/Identifiers.hpp"
#include "common/CardData.hpp"

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

//TODO: use in transmission ?!
struct CardData
{
	cardId id;
};

struct PlayerData
{
    int health;
    int energy;
};

#endif// _GAME_DATA_COMMON_HPP
