#ifndef _NON_BLOCKING_INPUT_HPP_
#define _NON_BLOCKING_INPUT_HPP_

#include <string>

class NonBlockingInput
{
public:
	/// Constructor
	NonBlockingInput(bool appendMode=false);

	/// This function is called to know if something is typed on stdin in the \a seconds following seconds
	/// \param seconds The number of seconds to wait before telling whether or not something was typed
	/// \return True if data is available (then call receiveStdinData) and false otherwise
	bool waitForData(double seconds);

	/// This function is called if data was typed to "read" it
	std::string&& receiveStdinData();

	static constexpr unsigned long long int microSecondsInOneSecond{1000000ULL};
	int stdinFileDescriptor;

private:
	bool _append;
	std::string _buffer;
};

#endif  // _NON_BLOCKING_INPUT_HPP_
