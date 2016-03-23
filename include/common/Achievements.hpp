#ifndef _ACHIEVEMENTS_COMMON_HPP_
#define _ACHIEVEMENTS_COMMON_HPP_
//NOTE: the file should be achievement.hpp (singular)

// std-C++ headers
#include <vector>
#include <string>
// WizardPoker headers
#include "common/Identifiers.hpp"

/// Used to send to a client his progress about an achievement
/// \see AchievementData and ClientAchievement client classes
struct Achievement
{
	AchievementId id;
	bool unlocked;
	sf::Int64 currentProgress;
};

typedef std::vector<Achievement> AchievementList;

#endif  // _ACHIEVEMENTS_COMMON_HPP_
