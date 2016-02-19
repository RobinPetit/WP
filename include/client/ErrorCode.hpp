#ifndef _ERROR_CODE_CLIENT_HPP_
#define _ERROR_CODE_CLIENT_HPP_

// std-C++ headers
#include <cstdlib>
// WizardPoker headers
#include "common/ErrorCode.hpp"

/// enum used to define the return status of the client program
enum
{
	/// return code in case of unability to reach the server
	UNABLE_TO_CONNECT,
	///
	UNABLE_TO_LISTEN
};

#endif // _ERROR_CODE_CLIENT_HPP_
