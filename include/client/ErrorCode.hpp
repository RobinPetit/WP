#ifndef _ERROR_CODE_CLIENT_HPP_
#define _ERROR_CODE_CLIENT_HPP_

#include <cstdlib>  /* defines EXIT_SUCCESS */

enum ErrorCode
{
	CLIENT_OK=EXIT_SUCCESS,  /* start with succes (usually 0) */
	UNABLE_TO_CONNECT
};

#endif /* _ERROR_CODE_CLIENT_HPP_ */
