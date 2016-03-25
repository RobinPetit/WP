#ifndef _ACHIEVEMENT_COMMON_HPP_
#define _ACHIEVEMENT_COMMON_HPP_

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
	sf::Int64 currentProgress;
};

typedef std::vector<Achievement> AchievementList;

#endif  // _ACHIEVEMENT_COMMON_HPP_
