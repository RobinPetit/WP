#include "common/NotConnectedException.hpp"

NotConnectedException::NotConnectedException(const std::string& message):
	std::runtime_error(message)
{

}

NotConnectedException::NotConnectedException():
	std::runtime_error("Client is not connected")
{

}
