#include "common/UnableToConnectException.hpp"

UnableToConnectException::UnableToConnectException(const std::string& message):
	std::runtime_error(message)
{

}

UnableToConnectException::UnableToConnectException():
	std::runtime_error("Client is not connected")
{

}
