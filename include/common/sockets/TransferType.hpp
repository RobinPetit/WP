#ifndef _TRANSFER_TYPE_HPP_
#define _TRANSFER_TYPE_HPP_

#include <SFML/Network/Packet.hpp>

/// TransferType is an enum that is used to determine the purpose of a TransferedPacket
enum class TransferType : sf::Uint32
{
	/////////////// Chat
	/// Used in pre-chat communications (getting the addresses and ports)
	CHAT_PLAYER_IP,

	/// Used during the chat communications (precises the content of the packet is a string message)
	CHAT_MESSAGE,

	/// Used during the chat communications: tells that the other player wants to end the conversation
	CHAT_QUIT,

	/////////////// Connection && registration to server

	/// Used when the user authenticates to the server
	CONNECTION,

	/// Used when the user tried to authenticates but gives wrong identifiers
	WRONG_IDENTIFIERS,

	/// Used when the user tried to authenticates but is already connected
	ALREADY_CONNECTED,

	/// Used when a new user register to the server
	REGISTERING,

	/// Used when a new user wants to register but the username is already taken
	USERNAME_NOT_AVAILABLE,

	/// Used when a new user wants to register but another error than GAME_USERNAME_NOT_AVAILABLE occurred
	FAILED_TO_REGISTER,

	/// Used when the client quits to tell the server it disconnects
	DISCONNECTION,

	/////////////// Game

	/// Used when a player asks to find an opponent
	GAME_REQUEST,

	/// Used when a player asks to leave the lobby
	GAME_CANCEL_REQUEST,

	/// Used when a new game is started when the server thread creates the different connections
	NEW_GAME_SERVER_CONNECTION,

	/// Used when the server tells the player the game is setup correctly and can begin
	GAME_STARTING,

	/// Used when the game starts and the client gives to the server the name of the deck he is going to use
	GAME_PLAYER_GIVE_DECK_NAMES,

	/// Used when the server tells the client its turn begins
	GAME_PLAYER_ENTER_TURN,

	/// Used when the server tells the client its turn ends
	GAME_PLAYER_LEAVE_TURN,

	/// Used to tell the client the game is over: one player won, the other failed
	GAME_OVER,

	/// Used to tell the client that he cannot play cards during this turn anymore
	GAME_CARD_LIMIT_TURN_REACHED,

	/// Used to tell the client that its energy is unsufficient to cast a spell or a monster
	GAME_NOT_ENOUGH_ENERGY,

	/// Used to tell the client that its energy points have been updated
	GAME_PLAYER_ENERGY_UPDATED,

	/// Used to tell the client that its health points have been udpated
	GAME_PLAYER_HEALTH_UPDATED,

	/// Used to tell the client that its opponent's health points have been updated
	GAME_OPPONENT_HEALTH_UPDATED,

	/// Used when sending to the client the current state of the board
	GAME_BOARD_UPDATED,

	/// Used when sending to the client the state of its opponent's board
	GAME_OPPONENT_BOARD_UPDATED,

	/// Used when sending to the client the current state of its graveyard
	GAME_GRAVEYARD_UPDATED,

	/// Used when sending to the client the current state of its hand
	GAME_HAND_UPDATED,

	/////////////// In-game player actions (client->server)

	/// Used when the user want to use a card in a game
	GAME_USE_CARD,

	/// Used when the user want to attack with one of its creatures
	GAME_ATTACK_WITH_CREATURE,

	/// Used when the player quits while playing a game
	GAME_QUIT_GAME,

	/////////////// Client/Server

	/// Used when a client checks if another client is connected
	CHECK_PRESENCE,

	/// Used when a client asks the list of his friends
	ASK_FRIENDS,

	/// Used when a client asks another player to be his friend
	NEW_FRIEND,

	/// Used when a player remove another player from its friend list
	REMOVE_FRIEND,

	/// Used as an "error-value" for acknowledgement of friendship requests
	NOT_EXISTING_FRIEND,

	/// Used when a client answers a friendship request
	RESPONSE_FRIEND_REQUEST,

	/// Used when a client asks what friendship requests he got
	GET_FRIEND_REQUESTS,

	/////////////// Cards managment

	/// Sent when the user wants its decks list
	ASK_DECKS_LIST,

	/// Sent when the user wants to change the content of a deck
	EDIT_DECK,

	/// Sent when the user wants to create a deck
	CREATE_DECK,

	/// Sent when the user wants to delete a deck
	DELETE_DECK,

	/// Sent when the user wants its cards collection
	ASK_CARDS_COLLECTION,

	/// Sent when the user wants the ladder
	ASK_LADDER,

	/////////////// General headers

	/// Used to acknowledge to client something happened correctly
	ACKNOWLEDGE,

	/// Used as a "false-valued boolean" for actions requested by the client
	FAILURE,
};

/// Overloading of the sf::Packet operators so that a TransferType variable can
/// be stored in a packet to be sent on the network
sf::Packet& operator<<(sf::Packet& packet, const TransferType& type);
sf::Packet& operator>>(sf::Packet& packet, TransferType& type);

#endif  // _TRANSFER_TYPE_HPP_
