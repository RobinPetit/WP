#ifndef _GAME_DATA_COMMON_HPP
#define _GAME_DATA_COMMON_HPP

#include <vector>
#include <string>
#include "common/Identifiers.hpp"

/// This struct is used to communicate changes of the board on the network.
/// Each time a player do an action while it is playing its turn, the server
/// sends an instance of this struct to the other player to notify it about
/// the actions of the opponent.
/// This is possible to optimize out the space by using union member, but
/// since there are std::vector involved, the union management is really
/// complicated (although really interesting to write) and just
/// obfuscates the code. So we loose a bit of space but save a lot of time.

typedef int64_t CardID;

struct BoardCreatureData
{
	CardID id;
	int health;
	int attack;
	int shield;
	std::string shieldType;
	bool isParalyzed;
};

struct GameData
{
	// TODO Use ClientCard::ID rather than int
	/*
	bool _thisPlayerRemainingCardsChanged;
	bool _thisPlayerEnergyChanged;
	bool _opponentEnergyChanged;
	bool _thisPlayerHealthChanged;
	bool _opponentHealthChanged;
	bool _turnChanged; //USE OTHER INFORMATIONS
	bool _isThisPlayerTurn;

	*/
	std::vector<CardID> _selfHandIDs;
	std::vector<CardID> _selfGraveIDs;
	std::size_t _oppoHandSize;
	std::size_t _selfDeckSize;

	std::vector<BoardCreatureData> _selfBoardIDs;
	std::vector<BoardCreatureData> _oppoBoardIDs;

	int _selfEnergy;
	int _oppoEnergy;

	int _selfHealth;
	int _oppoHealth;
};

#endif// _GAME_DATA_COMMON_HPP
