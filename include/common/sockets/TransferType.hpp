#ifndef _TRANSFER_TYPE_HPP_
#define _TRANSFER_TYPE_HPP_

#include <SFML/Network/Packet.hpp>

/// TransferType is an enum that is used to determine the purpose of a TransferedPacket
enum class TransferType : sf::Uint32
{
	// CHAT
	/// Used in pre-chat communications (getting the addresses and ports)
	CHAT_PLAYER_IP,
	/// Used during the chat communications (precises the content of the packet is a string message)
	CHAT_MESSAGE,
	/// Used during the chat communications: tells that the other player wants to end the conversation
	CHAT_QUIT,
	// GAME
	/// Not used yet, will be when the game core will be coded
	GAME_CONNECTION,
	// Client/Server
	/// Used when the client quits to tell the server it disconnects
	PLAYER_DISCONNECTION,
	PLAYER_CHECK_CONNECTION,
	PLAYER_ASKS_FRIENDS,
	PLAYER_NEW_FRIEND,
	NOT_EXISTING_FRIEND,
	PLAYER_RESPONSE_FRIEND_REQUEST,
	PLAYER_GETTING_FRIEND_REQUESTS_STATE,
};

/// Overloading of the sf::Packet operators so that a TransferType variable can be stored
/// in a packet to be sent on the network
sf::Packet& operator<<(sf::Packet& packet, const TransferType& type);
sf::Packet& operator>>(sf::Packet& packet, TransferType& type);

#endif  // _TRANSFER_TYPE_HPP_
