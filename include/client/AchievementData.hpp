#ifndef _ACHIEVEMENT_DATA_CLIENT_HPP
#define _ACHIEVEMENT_DATA_CLIENT_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/Identifiers.hpp"

struct AchievementData
{
	AchievementId id;
	std::string name;
	std::string description;
	sf::Int64 progressRequired; // sf::Int64 to be able to operate with Achievement::currentProgress
};

#endif // _ACHIEVEMENT_DATA_CLIENT_HPP
