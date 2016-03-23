#ifndef _CLIENT_ACHIEVEMENT_CLIENT_HPP
#define _CLIENT_ACHIEVEMENT_CLIENT_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/Identifiers.hpp"
#include "common/Achievements.hpp"
#include "client/AchievementData.hpp"

class ClientAchievement
{
	AchievementData _achievementData;
	const Achievement _achievement;
	
public:
	explicit ClientAchievement(const Achievement); // TODO: reference? pointer? move?...?
	
	std::string getName() const;
	std::string getPrettyName() const;
	std::string getDescription() const;
	int getRequiredProgress() const;
	int getCurrentProgress() const;
	float getProgressPercentage() const;
	
	bool isUnlocked() const;
};

#endif // _CLIENT_ACHIEVEMENT_CLIENT_HPP
