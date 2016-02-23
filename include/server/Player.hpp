#ifndef _PLAYER_HPP
#define _PLAYER_HPP

// std-C++ headers
#include <stack>
#include <random>
#include <array>
#include <functional>
#include <cstddef>
// WizardPoker headers
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Constraints.hpp"
#include "server/CardData.hpp"
// SFML headers
#include <SFML/Network/TcpSocket.hpp>

class Board;
class Creature;

/// Represents one of the two players for a given game.
class Player
{
public:
	typedef std::size_t ID;
	/// Constructor
	Player(Player::ID id, sf::TcpSocket& socket);
	void setOpponent(Player* opponent);  // Complementary

	/// Destructor.
	~Player() = default;

	/// Interface for basic gameplay (board)
	void enterTurn(int turn);
	void leaveTurn(int turn);
	void useCard(int handIndex); 	///< Use a card
	void attackWithCreature(int boardIndex, int victim);  ///< Attack victim with a card

	Player::ID getID();

	/// Effects
	void setConstraint(const EffectParamsCollection& args);
	void pickDeckCards(const EffectParamsCollection& args);
	void loseHandCards(const EffectParamsCollection& args);
	void reviveBinCard(const EffectParamsCollection& args);

	void stealHandCard(const EffectParamsCollection& args);
	void exchgHandCard(const EffectParamsCollection& args);

	void setEnergy(const EffectParamsCollection& args);
	void changeEnergy(const EffectParamsCollection& args);
	void changeHealth(const EffectParamsCollection& args);

	static std::function<void(Player&, const EffectParamsCollection&)> effectMethods[P_EFFECTS_COUNT];

	///Pass along effects to creatures
	void applyEffectToCreatures(int method, const EffectParamsCollection& effectArgs);
	void applyEffectToCreature(int boardIndex, int method, const EffectParamsCollection& effectArgs);
	int getTeamConstraint(int constraintID);

private:
	//////////// type definitions
	struct TurnData
	{
		int cardsUsed;
		int creaturesPlaced;
		int creatureAttacks;
		int spellCalls;
	};

	//////////// attributes
	Board* _board;
	Player* _opponent = nullptr;
	Player::ID _id;

	int _energy;
	int _health;

	sf::TcpSocket& _socketToClient;

	// Constraints
	Constraints _constraints = Constraints(P_CONSTRAINT_DEFAULTS, P_CONSTRAINTS_COUNT);

	constexpr static TurnData _emptyTurnData = {0, 0, 0, 0};
	TurnData _turnData;

	// Card holders
	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)

	// random management
	std::default_random_engine _engine;


	//////////// Private methods
	void exploitCardEffects(Card* usedCard);

	void cardDeckToHand(int amount);
	void cardHandToBoard(int handIndex);
	void cardHandToBin(int handIndex);  ///< Move the card at handIndex from the player's hand to the bin
	void cardBoardToBin(int boardIndex);  ///< Move the card at boardIndex from the board to the bin
	void cardBinToHand(int binIndex);
	void cardAddToHand(Card* given);
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);
	Card* cardExchangeFromHand(Card* given, int handIndex);
};


#endif  // _PLAYER_HPP
