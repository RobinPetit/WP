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
	/// Used when the user authenticates to the server
	GAME_CONNECTION,

	/// Used when the user tried to authenticates but gives wrong identifiers
	GAME_WRONG_IDENTIFIERS,

	/// Used when the user tried to authenticates but is already connected
	GAME_ALREADY_CONNECTED,

	/// Used when a new user register to the server
	GAME_REGISTERING,

	/// Used when a new user wants to register but the username is already taken
	GAME_USERNAME_NOT_AVAILABLE,

	/// Used when a new user wants to register but another error than GAME_USERNAME_NOT_AVAILABLE occurred
	GAME_FAILED_TO_REGISTER,

	/// Used when the server successfuly connected or registered the user
	GAME_CONNECTION_OR_REGISTERING_OK,

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

	/// Used when a player remove another player from its friend list
	PLAYER_REMOVE_FRIEND,

	/// Used as an "error-value" for acknowledgement of friendship requests
	NOT_EXISTING_FRIEND,

	/// Used when a client answers a friendship request
	PLAYER_RESPONSE_FRIEND_REQUEST,

	/// Used when a client asks what friendship requests he got
	PLAYER_GETTING_FRIEND_REQUESTS,

	/// Used to acknowledge the client that the client request succeeded, it is used for all requests
	PLAYER_ACKNOWLEDGE,

	/// Sent when the user wants its decks list
	PLAYER_ASKS_DECKS_LIST,

	/// Sent when the user wants to change the content of a deck
	PLAYER_EDIT_DECK,

	/// Sent when the user wants to create a deck
	PLAYER_CREATE_DECK,

	/// Sent when the user wants to delete a deck
	PLAYER_DELETE_DECK,

	/// Sent when the user wants its cards collection
	PLAYER_ASKS_CARDS_COLLECTION,

	/// Sent when the user wants the ladder
	PLAYER_ASKS_LADDER,

	/// Sent in case of success, from the server
	ACKNOWLEDGE,

	/// Sent in case of error, from the server
	FAILURE,
};

/// Overloading of the sf::Packet operators so that a TransferType variable can be stored
/// in a packet to be sent on the network
sf::Packet& operator<<(sf::Packet& packet, const TransferType& type);
sf::Packet& operator>>(sf::Packet& packet, TransferType& type);

#endif  // _TRANSFER_TYPE_HPP_
