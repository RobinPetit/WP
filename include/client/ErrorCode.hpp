#ifndef _ERROR_CODE_CLIENT_HPP_
#define _ERROR_CODE_CLIENT_HPP_

// std-C++ headers
#include <cstdlib>

/// enum used to define the return status of the client program
enum ErrorCode
{
	/// return code for a successful client ending program
	CLIENT_OK=EXIT_SUCCESS,  // start with succes (usually 0)
	/// return code in case of unability to reach the server
	UNABLE_TO_CONNECT
};

#endif // _ERROR_CODE_CLIENT_HPP_
