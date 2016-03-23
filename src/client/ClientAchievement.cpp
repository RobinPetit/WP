#include "client/ClientAchievement.hpp"

// WizardPoker headers
#include "common/Achievements.hpp"
#include "client/AchievementData.hpp"

ClientAchievement::ClientAchievement(const Achievement achievement, const AchievementData achievementData) :
	_achievement(achievement),
	_achievementData(achievementData)
{
}

std::string ClientAchievement::getName() const
{
	return "TODO: ClientAchievement::getName";
}

std::string ClientAchievement::getPrettyName() const
{
	return "TODO: ClientAchievement::getPrettyName";
}

std::string ClientAchievement::getDescription() const
{
	return "TODO: ClientAchievement::getDescription";
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
	return static_cast<float>(getCurrentProgress()*100) / getRequiredProgress();
}

bool ClientAchievement::isUnlocked() const
{
	return false; ///\TODO
}
