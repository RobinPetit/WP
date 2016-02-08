#ifndef _ERROR_CODE_SERVER_HPP_
#define _ERROR_CODE_SERVER_HPP_

#include <cstdlib>  /* defines EXIT_SUCCESS */

/* enum containing an ID for the server return code */
enum ErrorCode
{
	SERVER_OK=EXIT_SUCCESS,  /* start with succes (usually 0) */
	UNABLE_TO_LISTEN
};

#endif /* _ERROR_CODE_SERVER_HPP_ */
