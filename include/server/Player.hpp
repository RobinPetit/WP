#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <SFML/System.hpp>
#include <cstdlib>
#include <stack>
#include "server/Card.hpp"
#include "server/Spell.hpp"
#include "server/Creature.hpp"
#include "server/Constraints.hpp"

class Board;

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
	std::vector<Creature*> getBoardCreatures() { return _cardBoard; };

	/// Effects
	void setConstraint(const std::vector<unsigned>& args);
	void pickDeckCards(const std::vector<unsigned>& args);
	void loseHandCards(const std::vector<unsigned>& args);
	void stealHandCard(const std::vector<unsigned>& args);
	void exchgHandCard(const std::vector<unsigned>& args);
	//void reviveBinCard(std::vector<unsigned>& args);
	void setEnergyPoints(const std::vector<unsigned>& args);
	void addEnergyPoints(const std::vector<unsigned>& args);
	void subEnergyPoints(const std::vector<unsigned>& args);
	void addLifePoints(const std::vector<unsigned>& args);
	void subLifePoints(const std::vector<unsigned>& args);

	void (Player::*effectMethods[P_EFFECTS_COUNT])(std::vector<unsigned>&) =
	{
		&Player::setConstraint,
		&Player::pickDeckCards,
		&Player::loseHandCards,
		&Player::stealHandCard,
		&Player::exchgHandCard,
		//&Player::reviveBinCard,
		&Player::setEnergyPoints,
		&Player::addEnergyPoints,
		&Player::subEnergyPoints,
		&Player::addLifePoints,
		&Player::subLifePoints,
	};


private:
	void exploitCardEffects(Card* usedCard);

	void cardPickFromDeck(unsigned amount);
	void cardPlaceOnBoard(unsigned handIndex);
	void cardDiscardFromHand(unsigned handIndex);  /// Move the card at handIndex from the player's hand to the bin
	void cardDiscardFromBoard(unsigned boardIndex);  /// Move the card at boardIndex from the board to the bin
	Card* cardRemoveFromHand();
	Card* cardExchangeFromHand(Card* given);

	std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
	std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
	std::vector<Creature *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
	std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)

	unsigned _energyPoints;
	unsigned _healthPoints;

	Board* _board;
	Player* _opponent = nullptr;
	unsigned _id;

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
};


#endif// _PLAYER_HPP
