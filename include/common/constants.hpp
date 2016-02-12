#ifndef _COMMON_CONSTANTS_HPP_
#define _COMMON_CONSTANTS_HPP_

// SFML headers
#include <SFML/System/Time.hpp>

/* keep in mind that ports below 1K are usually reserved.
 * The port is represented by a 16-bit unsigned value
 * (in range 0 to 0xFFFF)
 */
#define SERVER_PORT 0x4000  // totally arbitrary, if for a reason it does not fit, change it

// as long as no server is truly available, let's keep the server in localhost.
#define SERVER_ADDRESS "localhost"

// to have secured socket transfers, let's set a boundary to the player's name
#define MAX_NAME_LENGTH 0x40  // 64 chars seems a good (but also arbitrary) high limit

// sf::Time value used to sf::sleep when a waiting duration is needed
#define SOCKET_TIME_SLEEP sf::milliseconds(250)

// location of the log files of the chat
#define CHAT_LOGGERS_DIR "chat/logs/"

#endif // _COMMON_CONSTANTS_HPP
