#ifndef _CLIENT_INFORMATIONS_HPP_
#define _CLIENT_INFORMATIONS_HPP_

// SFML headers
#include <SFML/Network/TcpSocket.hpp>
// std-C++ headers
#include <vector>
#include <string>

/// structure used inside of the server program to keep informations
/// on a single client
struct ClientInformations
{
	/// Holds the connection with the client. The smart pointers helps closing
	/// the connection when needed, so that when the ClientInformations instance
	/// gets deleted, the connection is closed (but a simple instance of socket
	/// is not sufficient, because we need a dynamic allocation).
	std::unique_ptr<sf::TcpSocket> socket;
	sf::Uint16 listeningPort;  ///< used to send connection for the chat
	std::vector<std::string> friendshipRequests;  ///< players client asked to become friends with
	std::vector<std::string> acceptedRequests;  ///< players who accepted the friendship request
	std::vector<std::string> refusedRequests;  ///< players who refused the friendship request
	std::vector<std::string> externalRequests;  ///< players who asked to become friend with client
	std::vector<std::string> friends;
	std::size_t id;
};

#endif  // _CLIENT_INFORMATIONS_HPP_
