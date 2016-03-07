#ifndef _IDENTIFIERS_COMMON_HPP
#define _IDENTIFIERS_COMMON_HPP

#include <SFML/System.hpp>

/// The identifier of a card, this is the identifier that is encoded in the
/// database, and the only information used to transmit a card on the network.
typedef sf::Int64 cardId;

/// The identifier of a deck, this is the identifier that is encoded in the
/// database, and the only information used to transmit a deck on the network.
typedef sf::Int64 deckId;

/// The identifier of a card, this is the identfier that is encoded in the
/// database, and is used by the class Server to identify an user.
typedef sf::Int64 userId;

#endif  // _IDENTIFIERS_COMMON_HPP
