#include "client/ClientAchievement.hpp"

// std-C++ headers
#include <cassert>
// WizardPoker headers
#include "common/Achievements.hpp"
#include "client/AchievementData.hpp"

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
	return getName() + ' ' + (_unlocked ? "✔" : std::string("(") + getProgressPercentage() + ")" );
}

std::string ClientAchievement::getDescription() const
{
	return _achievementData.description;
}

int ClientAchievement::getRequiredProgress() const
{
	return 42; ///\TODO
}

int ClientAchievement::getCurrentProgress() const
{
	return 4; ///\TODO
}

float ClientAchievement::getProgressPercentage() const
{
	return static_cast<float>(static_cast<double>(getCurrentProgress()*100) / getRequiredProgress());
}

bool ClientAchievement::isUnlocked() const
{
	return false; ///\TODO
}
