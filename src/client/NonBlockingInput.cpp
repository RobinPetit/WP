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
	// needed for the select function
	fd_set descriptorsSet;
	// clear the set
	FD_ZERO(&descriptorsSet);
	// and place STDIN in it
	FD_SET(stdinFileDescriptor, &descriptorsSet);
	// precise how long to wait
	struct timeval waitingDelay;
	waitingDelay.tv_sec = static_cast<time_t>(seconds);
	waitingDelay.tv_usec = static_cast<suseconds_t>((seconds - static_cast<int>(seconds)) * microSecondsInOneSecond);
	// and then wait for an input
	// select return -1 if error occured (\TODO: handle it), 0 if no data was received and > 0 if data was typed
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

// consider the buffer like temporary. Then once it is used, it disappears (like a *real* IO stream buffer).
std::string&& NonBlockingInput::receiveStdinData()
{
	return std::move(_buffer);
}

#else
# error "No other system than Linux is fully supported for now. Compile on Linux for Linux"
#endif
