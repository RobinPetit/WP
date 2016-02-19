#ifndef _PLAYER_HPP
#define _PLAYER_HPP

// std-C++ headers
#include <stack>
#include <random>
#include <cstddef>
// WizardPoker headers
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Constraints.hpp"

class Board;
class Creature;

/// Represents one of the two players for a given game.
class Player
{
public:
	typedef void (Player::*PlayerEffectMethod)(const EffectParamsCollection&);
	typedef std::size_t ID;
	/// Constructor
	Player(Player::ID id);
	void setOpponent(Player* opponent);  // Complementary

	/// Destructor.
	~Player() = default;

	/// Interface for basic gameplay (board)
	void enterTurn(unsigned turn);
	void leaveTurn(unsigned turn);
	void useCard(unsigned handIndex); 	///< Use a card
	void attackWithCreature(unsigned boardIndex, unsigned victim);  ///< Attack victim with a card

	/// Effects
	void setConstraint(const EffectParamsCollection& args);
	void pickDeckCards(const EffectParamsCollection& args);
	void loseHandCards(const EffectParamsCollection& args);
	void reviveBinCard(const EffectParamsCollection& args);

	void stealHandCard(const EffectParamsCollection& args);
	void exchgHandCard(const EffectParamsCollection& args);

	void setEnergyPoints(const EffectParamsCollection& args);
	void addEnergyPoints(const EffectParamsCollection& args);
	void subEnergyPoints(const EffectParamsCollection& args);
	void addLifePoints(const EffectParamsCollection& args);
	void subLifePoints(const EffectParamsCollection& args);

	PlayerEffectMethod effectMethods[P_EFFECTS_COUNT] =
	{
		&Player::setConstraint,
		&Player::pickDeckCards,
		&Player::loseHandCards,
		&Player::reviveBinCard,

		&Player::stealHandCard,
		&Player::exchgHandCard,

		&Player::setEnergyPoints,
		&Player::addEnergyPoints,
		&Player::subEnergyPoints,
		&Player::addLifePoints,
		&Player::subLifePoints,
	};

	///Pass along effects to creatures
	void applyEffectToCreatures(unsigned method, const EffectParamsCollection& effectArgs);
	void applyEffectToCreature(unsigned boardIndex, unsigned method, const EffectParamsCollection& effectArgs);
	std::pair<unsigned, unsigned> getTeamConstraint(unsigned constraintID);

private:
	Board* _board;
	Player* _opponent = nullptr;
	Player::ID _id;

	unsigned _energyPoints;
	unsigned _healthPoints;

	//Constraints
	ConstraintList _constraints = ConstraintList(P_CONSTRAINT_DEFAULTS, P_CONSTRAINTS_COUNT);

	struct TurnData
	{
		unsigned cardsUsed;
		unsigned creaturesPlaced;
		unsigned creatureAttacks;
		unsigned spellCalls;
	};

	constexpr static TurnData _emptyTurnData = {0, 0, 0, 0};
	TurnData _turnData;

	//Card holders
	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)

	std::default_random_engine _engine;

	//Private methods
	void exploitCardEffects(Card* usedCard);

	void cardDeckToHand(unsigned amount);
	void cardHandToBoard(unsigned handIndex);
	void cardHandToBin(unsigned handIndex);  ///< Move the card at handIndex from the player's hand to the bin
	void cardBoardToBin(unsigned boardIndex);  ///< Move the card at boardIndex from the board to the bin
	void cardBinToHand(unsigned binIndex);
	void cardAddToHand(Card* given);
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);
	Card* cardExchangeFromHand(Card* given, unsigned handIndex);
};


#endif  // _PLAYER_HPP
