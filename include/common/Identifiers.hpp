#ifndef _IDENTIFIERS_COMMON_HPP
#define _IDENTIFIERS_COMMON_HPP

#include <SFML/Network/Packet.hpp>

/// The identifier of a card, this is the identifier that is encoded in the
/// database, and the only information used to transmit a card on the network.
typedef sf::Int64 CardId;

/// The identifier of a deck, this is the identifier that is encoded in the
/// database, and the only information used to transmit a deck on the network.
typedef sf::Int64 deckId;

/// The identifier of a card, this is the identfier that is encoded in the
/// database, and is used by the class Server to identify an user.
typedef sf::Int64 userId;

/// The identifier of an achievement, this is the identifier that is encoded in the
/// database, and is used by the class Client to identify an achievement.
typedef sf::Int64 AchievementId; //See how beautiful it is with the upper case at the beginning ?

#endif  // _IDENTIFIERS_COMMON_HPP
