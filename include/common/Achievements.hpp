#ifndef _ACHIEVEMENTS_COMMON_HPP_
#define _ACHIEVEMENTS_COMMON_HPP_

// std-C++ headers
#include <vector>
#include <string>

struct Achievement
{
	std::string name;
	std::string description;
	bool unlocked;
	bool secret;
};

typedef std::vector<Achievement> AchievementList;

#endif  // _ACHIEVEMENTS_COMMON_HPP_
