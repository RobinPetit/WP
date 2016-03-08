#ifndef _END_GAME_CAUSE_HPP_
#define _END_GAME_CAUSE_HPP_

#include <SFML/Network/Packet.hpp>

/// A struct that indicates why (and how) a game ended.
/// The applyToSelf variable works as follow:
/// if cause is OUT_OF_HEALTH and applyToSelf is true, then the game ended
/// because the player who receive this message ran out of health.
/// But if applyToSelf is false, then the game ended because the opponent
/// ran out of health.
///
/// \note applyToSelf also indicates who won the game: if applyToSelf is true, then
/// that is the opponent who won, and vice versa.
///
/// The structure of this struct could me made simpler, but then the code that
/// will use this struct would be more complicated. The attribute applyToSelf,
/// although not really intuitive, is programmatically useful.
struct EndGame
{
	/// Describes the effective cause of the end of the game, whithout precising
	/// to which player this applies
	enum class Cause : sf::Int32
	{
		TEN_TURNS_WITH_EMPTY_DECK,
		OUT_OF_HEALTH,
		QUITTED,
	};

	/// \see Cause
	Cause cause;

	/// True if cause applies to self, false if it applies to the opponent.
	bool applyToSelf;
};

#endif  // _END_GAME_CAUSE_HPP_
