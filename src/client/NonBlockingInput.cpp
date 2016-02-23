#include "client/NonBlockingInput.hpp"

#if defined __linux__
extern "C"
{
#include <sys/select.h>  // non blocking inputs (C lib)
}

// std-C++ headers
#include <cstdio>  // STDIN_FILENO
#include <iostream>  // std::getline && std::cin
#include <cassert>

NonBlockingInput::NonBlockingInput(bool appendMode):
	stdinFileDescriptor{fileno(stdin)},
	_append{appendMode}
{

}

bool NonBlockingInput::waitForData(double seconds)
{
	fd_set descriptorsSet;
	FD_ZERO(&descriptorsSet);
	FD_SET(stdinFileDescriptor, &descriptorsSet);
	struct timeval waitingDelay;
	waitingDelay.tv_sec = static_cast<time_t>(seconds);
	waitingDelay.tv_usec = static_cast<suseconds_t>((seconds - static_cast<int>(seconds)) * microSecondsInOneSecond);
	if(select(stdinFileDescriptor+1, &descriptorsSet, nullptr, nullptr, &waitingDelay) == 0)
		return false;
	// as stdin is the only file in the set, only itself may have received data
	assert(FD_ISSET(stdinFileDescriptor, &descriptorsSet));
	if(!_append)
		std::getline(std::cin, _buffer);
	else
	{
		std::string tmp;
		std::getline(std::cin, tmp);
		_buffer += tmp;
	}
	return true;
}

std::string&& NonBlockingInput::receiveStdinData()
{
	return std::move(_buffer);
}

#else
# error "No other system than Linux is fully supported for now. Compile on Linux for Linux"
#endif
