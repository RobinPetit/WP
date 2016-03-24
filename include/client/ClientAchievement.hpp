#ifndef _CLIENT_ACHIEVEMENT_CLIENT_HPP
#define _CLIENT_ACHIEVEMENT_CLIENT_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/Identifiers.hpp"
#include "common/Achievements.hpp"


/// created by ClientDatabase::getAchievementData(AchievementId id) (\TODO)
struct AchievementData
{
	AchievementId id;
	std::string name;
	std::string description;
	sf::Int64 requiredProgress; // sf::Int64 to be able to operate with Achievement::currentProgress
};

class ClientAchievement
{
	const AchievementData _achievementData;
	const Achievement _achievement;

public:
	explicit ClientAchievement(const Achievement, const AchievementData); // TODO: reference? pointer? move?...?

	std::string getName() const;
	std::string getPrettyName() const;
	std::string getDescription() const;
	int getRequiredProgress() const;
	int getCurrentProgress() const;

	bool isUnlocked() const;
};

typedef std::vector<ClientAchievement> ClientAchievementList;

#endif // _CLIENT_ACHIEVEMENT_CLIENT_HPP
