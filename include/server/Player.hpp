#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <SFML/System.hpp>
#include <cstdlib>
#include <stack>
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Constraints.hpp"

class Board;
class Creature;

/// Represents one of the two players for a given game.
class Player
{
public:
	/// Constructor
	Player(unsigned id);
	void setOpponent(Player* opponent); //Complementary

	/// Destructor.
	~Player() = default;

	/// Interface for basic gameplay (board)
	void enterTurn(unsigned turn);
	void leaveTurn(unsigned turn);
	void useCard(unsigned handIndex); 	/// Use a card
	void attackWithCreature(unsigned boardIndex, unsigned victim);  /// Attack victim with a card

	/// Effects
	void setConstraint(const std::vector<unsigned>& args);
	void pickDeckCards(const std::vector<unsigned>& args);
	void loseHandCards(const std::vector<unsigned>& args);
	void reviveBinCard(const std::vector<unsigned>& args);

	void stealHandCard(const std::vector<unsigned>& args);
	void exchgHandCard(const std::vector<unsigned>& args);

	void setEnergyPoints(const std::vector<unsigned>& args);
	void addEnergyPoints(const std::vector<unsigned>& args);
	void subEnergyPoints(const std::vector<unsigned>& args);
	void addLifePoints(const std::vector<unsigned>& args);
	void subLifePoints(const std::vector<unsigned>& args);

	void (Player::*effectMethods[P_EFFECTS_COUNT])(const std::vector<unsigned>&) =
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
    void applyEffectToCreatures(unsigned method, const std::vector<unsigned>& effectArgs);
    void applyEffectToCreature(unsigned boardIndex, unsigned method, const std::vector<unsigned>& effectArgs);
    std::pair<unsigned, unsigned> getTeamConstraint(unsigned constraintID);

private:
	Board* _board;
	Player* _opponent = nullptr;
	unsigned _id; //@RobinPetit change this to whatever helps communicate over network

	unsigned _energyPoints;
	unsigned _healthPoints;

	//Constraints
	ConstraintList _constraints = ConstraintList(P_CONSTRAINT_DEFAULTS, P_CONSTRAINTS_COUNT);

	struct TurnData
	{
		unsigned cardsUsed=0;
		unsigned creaturesPlaced=0;
		unsigned creatureAttacks=0;
		unsigned spellCalls=0;
	} _emptyTurnData;

	TurnData _turnData;

	//Card holders
	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)

	//Private methods
	void exploitCardEffects(Card* usedCard);

	void cardDeckToHand(unsigned amount);
	void cardHandToBoard(unsigned handIndex);
	void cardHandToBin(unsigned handIndex);  /// Move the card at handIndex from the player's hand to the bin
	void cardBoardToBin(unsigned boardIndex);  /// Move the card at boardIndex from the board to the bin
	void cardBinToHand(unsigned binIndex);
	void cardAddToHand(Card* given);
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);
	Card* cardExchangeFromHand(Card* given, unsigned handIndex);
};


#endif// _PLAYER_HPP
