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
	/// Used when a player asks to find an opponent
	GAME_REQUEST,
	// Client/Server
	/// Used when the client quits to tell the server it disconnects
	PLAYER_DISCONNECTION,
	/// Used when a client checks if another client is connected
	PLAYER_CHECK_CONNECTION,
	/// Used when a client asks the list of his friends
	PLAYER_ASKS_FRIENDS,
	/// Used when a client asks another player to be his friend
	PLAYER_NEW_FRIEND,
	/// Used as an "error-value" for acknowledgement of friendship requests
	NOT_EXISTING_FRIEND,
	/// Used when a client answers a friendship request
	PLAYER_RESPONSE_FRIEND_REQUEST,
	/// Used when a client asks which of his own requests were accepted/refused
	PLAYER_GETTING_FRIEND_REQUESTS_STATE,
	/// Used when a client asks what friendship requests he got
	PLAYER_GETTING_FRIEND_REQUESTS,
};

/// Overloading of the sf::Packet operators so that a TransferType variable can be stored
/// in a packet to be sent on the network
sf::Packet& operator<<(sf::Packet& packet, const TransferType& type);
sf::Packet& operator>>(sf::Packet& packet, TransferType& type);

#endif  // _TRANSFER_TYPE_HPP_
