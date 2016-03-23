#ifndef _ACHIEVEMENTS_COMMON_HPP_
#define _ACHIEVEMENTS_COMMON_HPP_

// std-C++ headers
#include <vector>
#include <string>

class Achievement
{
private:
	std::string _name;
	std::string _description;
	bool _unlocked;
	std::size_t _progressCurrent, _progressRequired;
public:
	std::string getName() const;
	std::string getDescription() const;
};

typedef std::vector<Achievement> AchievementList;

#endif  // _ACHIEVEMENTS_COMMON_HPP_
