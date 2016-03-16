#ifndef _GAME_DATA_COMMON_HPP
#define _GAME_DATA_COMMON_HPP

#include "common/Identifiers.hpp"
#include "common/CardData.hpp"

/// These struct are used to communicate changes of the game data on the network (server => client).
enum class CardToSelect : sf::Int32
{
	SELF_BOARD,
	OPPO_BOARD,
	SELF_HAND,
	OPPO_HAND
};

struct BoardCreatureData
{
	cardId id;
	int health;
	int attack;
	int shield;
	std::string shieldType;
	//bool isParalyzed;
};

struct CardData
{
	cardId id;
};

#endif  // _GAME_DATA_COMMON_HPP
