#ifndef _NON_BLOCKING_INPUT_HPP_
#define _NON_BLOCKING_INPUT_HPP_

#include <string>

class NonBlockingInput
{
public:
	NonBlockingInput(bool appendMode=false);

	bool waitForData(double seconds);
	std::string&& receiveStdinData();

	static constexpr unsigned long long int microSecondsInOneSecond{1000000ULL};
	int stdinFileDescriptor;

private:
	bool _append;
	std::string _buffer;
};

#endif  // _NON_BLOCKING_INPUT_HPP_
