#ifndef _LADDER_HPP_
#define _LADDER_HPP_

#include <array>

struct LadderEntry
{
	std::string name;
	unsigned victories;
	unsigned defeats;
};

struct Ladder
{
	constexpr static unsigned size{20};
	std::array<LadderEntry, size> ladder;
	unsigned nbOfPlayers;
};

#endif  // _LADDER_HPP_
