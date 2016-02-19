#ifndef _ERROR_CODE_COMMON_HPP_
#define _ERROR_CODE_COMMON_HPP_

// To differentiate the error codes given here among the others, common return codes
// are set as negative values

/// enum used to define the error codes possible for both the client and the server
enum
{
	/// return code in case of non-existing config file
	NO_CONFIG_FILE=-2,
	/// return code for an incomplete config file
	WRONG_FORMAT_CONFIG_FILE=-1,
	SUCCESS=0,
};

#endif  // _ERROR_CODE_COMMON_HPP_
