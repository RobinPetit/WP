#ifndef _ACHIEVEMENTS_COMMON_HPP_
#define _ACHIEVEMENTS_COMMON_HPP_

// std-C++ headers
#include <vector>
#include <string>
// WizardPoker headers
#include "common/Identifiers.hpp"

struct Achievement
{
	AchievementId id;
	bool unlocked;
	sf::Int64 currentProgress;
};

typedef std::vector<Achievement> AchievementList;

#endif  // _ACHIEVEMENTS_COMMON_HPP_
