#ifndef _GAME_DATA_COMMON_HPP
#define _GAME_DATA_COMMON_HPP

// WizardPoker headers
#include "common/Identifiers.hpp"
#include "common/CardData.hpp"
// std-C++ headers
#include <array>

/// These struct are used to communicate changes of the game data on the network (server => client).
enum class CardToSelect : sf::Int32
{
	SELF_BOARD,
	OPPO_BOARD,
	SELF_HAND,
	/*OPPO_HAND,
	SELF_GRAVEYARD*/
};

struct BoardCreatureData
{
	CardId id;
	int health;
	int attack;
	int shield;
	std::string shieldType;
	//bool isParalyzed;

	static constexpr std::array<const char *, 4> shieldTypes =
	{
		"none",
		"blue",
		"orange",
		"legendary"
	};
};

struct CardData
{
	CardId id;
};

#endif  // _GAME_DATA_COMMON_HPP
