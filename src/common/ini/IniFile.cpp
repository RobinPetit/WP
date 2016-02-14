// WizardPoker headers
#include <common/ini/IniFile.hpp>
#include <common/ErrorCode.hpp>
// std-C++ headers
#include <fstream>

#define INI_COMMENT_DELIMITER ';'

IniFile::IniFile():
	std::unordered_map<std::string, std::string>()
{

}

int IniFile::readFromFile(const std::string& path)
{
	std::ifstream ini;
	ini.open(path);
	if(!ini)
		return NO_CONFIG_FILE;
	std::string line;
	while(std::getline(ini, line))
	{
		// ignore comment lines
		if(line[0] == INI_COMMENT_DELIMITER)
			continue;
		std::string::size_type splitPos = line.find('=');
		if(splitPos == std::string::npos)
			continue;  // if line has an error, ignore it
		std::string key(line.substr(0, splitPos));
		std::string value(line.substr(splitPos+1));
		(*this)[key] = value;
	}
	return SUCCESS;
}
