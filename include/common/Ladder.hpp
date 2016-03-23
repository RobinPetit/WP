#ifndef _LADDER_HPP_
#define _LADDER_HPP_

// std-C++ headers
#include <vector>
#include <string>

struct LadderEntry
{
	std::string name;
	unsigned victories;
	unsigned defeats;
};

typedef std::vector<LadderEntry> Ladder;

#endif  // _LADDER_HPP_
