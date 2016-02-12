#ifndef _CONFIG_PARSER_COMMON_HPP_
#define _CONFIG_PARSER_COMMON_HPP_

// SFML headers
#include <SFML/Config.hpp>
// std-C++ headers
#include <string>
// WizardPoker headers
#include <common/ConfigData.hpp>

class ConfigParser
{
public:
	/// \TODO description
	static int readFromFile(const std::string& path, ConfigData& data);
};

#endif  // _CONFIG_PARSER_COMMON_HPP_
