#include "client/ClientAchievement.hpp"

// std-C++ headers
#include <cassert>
#include <string>

ClientAchievement::ClientAchievement(const Achievement achievement, const AchievementData achievementData) :
	_achievementData(achievementData),
	_achievement(achievement)
{
	assert(_achievementData.id == _achievement.id);
}

std::string ClientAchievement::getName() const
{
	return _achievementData.name;
}

std::string ClientAchievement::getPrettyName() const
{
	return (isUnlocked() ? std::string("✔ ") : std::string("✘ "))
	       + getName()
	       + (isUnlocked() ? "" : std::string(" (") + std::to_string(getCurrentProgress()) + "/" + std::to_string(getRequiredProgress()) + ")");
}

std::string ClientAchievement::getDescription() const
{
	return _achievementData.description;
}

int ClientAchievement::getRequiredProgress() const
{
	return static_cast<int>(_achievementData.requiredProgress);
}

int ClientAchievement::getCurrentProgress() const
{
	return static_cast<int>(_achievement.currentProgress);
}

bool ClientAchievement::isUnlocked() const
{
	return getCurrentProgress() >= getRequiredProgress();
}
