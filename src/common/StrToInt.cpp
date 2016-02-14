#include <common/StrToInt.hpp>

#include <sstream>

int strToInt(const std::string& str)
{
	int ret;
	if(str[0] == '0')
	{
		if(str[1] == 'x')
			std::stringstream(str.substr(2)) >> std::hex >> ret;
		else
			std::stringstream(str.substr(1)) >> std::oct >> ret;
	}
	else
		std::stringstream(str) >> std::dec >> ret;
	return ret;
}
