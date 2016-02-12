// WizardPoker headers
#include <common/ConfigParser.hpp>
#include <common/ErrorCode.hpp>
// std-C++ headers
#include <string>
#include <sstream>
#include <fstream>

#define INI_COMMENT_DELIMITER ';'

// reads data from the file: the only data readable are:
//     + SERVER_ADDRESS (string);
//     + SERVER_PORT (hex integer).
// Any other value is ignored
int ConfigParser::readFromFile(const std::string& path, ConfigData& data)
{
	bool haveAddress(false), havePort(false);
	std::ifstream configFile;
	configFile.open(path);
	if(!configFile)
		return NO_CONFIG_FILE;
	std::string line;
	while(std::getline(configFile, line))
	{
		if(line[0] == INI_COMMENT_DELIMITER)
			continue;  // if the line is a comment, ignore it
		std::string::size_type splitPos = line.find('=');
		if(splitPos == std::string::npos)
			continue;  // if line has an error, ignore it
		// std::cout << line.substr(0, splitPos) << " = " << line.substr(splitPos+1) << std::endl;
		std::string key(line.substr(0, splitPos));
		if(key == "SERVER_ADDRESS")
		{
			haveAddress = true;
			data.address = line.substr(splitPos+1);
		}
		else if(key == "SERVER_PORT")
		{
			std::stringstream sstream;
			havePort = true;
			std::string value(line.substr(splitPos+1));
			if(value[0] == '0')
			{
				if(value[1] == 'x')
					std::stringstream(line.substr(splitPos+3)) >> std::hex >> data.port;
				else
					std::stringstream(line.substr(splitPos+2)) >> std::oct >> data.port;
			}
			else
				std::stringstream(line.substr(splitPos+1)) >> data.port;
		}
	}
	return (haveAddress && havePort) ? SUCCESS : WRONG_FORMAT_CONFIG_FILE;
}
