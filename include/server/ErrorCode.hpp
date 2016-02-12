#ifndef _ERROR_CODE_SERVER_HPP_
#define _ERROR_CODE_SERVER_HPP_

// std-C++ headers
#include <cstdlib>

/// enum used to define the return status of the server program
enum ErrorCode
{
	/// return code for a successful server program
	SERVER_OK=EXIT_SUCCESS,  // start with succes (usually 0)
	/// return code in case of non-free listening port
	UNABLE_TO_LISTEN
};

#endif // _ERROR_CODE_SERVER_HPP_
