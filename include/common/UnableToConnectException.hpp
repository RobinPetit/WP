#ifndef _UNABLE_TO_CONNECT_EXCEPTION_COMMON_HPP
#define _UNABLE_TO_CONNECT_EXCEPTION_COMMON_HPP

#include <stdexcept>
#include <string>

class UnableToConnectException : public std::runtime_error
{
public:
	UnableToConnectException(const std::string& message);
	UnableToConnectException();
};

#endif  // _UNABLE_TO_CONNECT_EXCEPTION_COMMON_HPP
