#ifndef _INI_FILE_COMMON_HPP_
#define _INI_FILE_COMMON_HPP_

#include <unordered_map>
#include <string>

class IniFile : public std::unordered_map<std::string, std::string>
{
public:
	IniFile();

	int readFromFile(const std::string& path);

private:
};

#endif  // _INI_FILE_COMMON_HPP_
