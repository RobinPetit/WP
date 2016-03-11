#ifndef _ERROR_CODE_SERVER_HPP_
#define _ERROR_CODE_SERVER_HPP_

// WizardPoker headers
#include "common/ErrorCode.hpp"

/// enum used to define the return status of the server program
enum
{
	/// return code in case of non-free listening port
	UNABLE_TO_LISTEN=1,
};

#endif // _ERROR_CODE_SERVER_HPP_
