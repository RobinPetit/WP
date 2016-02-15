#ifndef _NOT_CONNECTED_EXCEPTION_HPP
#define _NOT_CONNECTED_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class NotConnectedException : public std::runtime_error
{
public:
	NotConnectedException(const std::string& message);
	NotConnectedException();
};

#endif  // _NOT_CONNECTED_EXCEPTION_HPP
