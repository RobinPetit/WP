#ifndef _COMMON_CONSTANTS_HPP_
#define _COMMON_CONSTANTS_HPP_

// SFML headers
#include <SFML/System/Time.hpp>

// the file that contains informations about the server location
#define SERVER_CONFIG_FILE_PATH "./server.ini"

// to have secured socket transfers, let's set a boundary to the player's name
#define MAX_NAME_LENGTH 0x40  // 64 chars seems a good (but also arbitrary) high limit

// sf::Time value used to sf::sleep when a waiting duration is needed
#define SOCKET_TIME_SLEEP sf::milliseconds(250)

// location of the log files of the chat
#define CHAT_LOGGERS_DIR "chat/logs/"

#endif // _COMMON_CONSTANTS_HPP
