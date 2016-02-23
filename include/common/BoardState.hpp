#ifndef _BOARD_STATE_COMMON_HPP
#define _BOARD_STATE_COMMON_HPP

#include <vector>
// TODO Card.hpp should be in common/
#include "server/Card.hpp"

/// This struct is used to communicate changes of the board on the network.
/// Each time a player do an action while it is playing its turn, the server
/// sends an instance of this struct to the other player to notify it about
/// the actions of the opponent.
/// This is possible to optimize out the space by using union member, but
/// since there are std::vector involved, the union management is really
/// complicated (although really interesting to write) and just
/// obfuscates the code. So we loose a bit of space but save a lot of time.
struct BoardState
{
	// TODO Use Card::ID rather than int
	bool _thisPlayerCardsInHandChanged;
	std::vector<int> _thisPlayerCardsInHand;

	bool _opponentCardsInHandChanged;
	size_t _opponentCardsInHand;

	bool _thisPlayerCardsOnBoardChanged;
	std::vector<int> _thisPlayerCardsOnBoard;

	bool _opponentCardsOnBoardChanged;
	std::vector<int> _opponentCardsOnBoard;

	bool _thisPlayerRemainingCardsChanged;
	size_t _thisPlayerRemainingCards;

	bool _thisPlayerEnergyChanged;
	int _thisPlayerEnergy;

	bool _opponentEnergyChanged;
	int _opponentEnergy;

	bool _thisPlayerHealthChanged;
	int _thisPlayerHealth;

	bool _opponentHealthChanged;
	int _opponentHealth;

	bool _turnChanged;
	bool _isThisPlayerTurn;
};

#endif// _BOARD_STATE_COMMON_HPP
