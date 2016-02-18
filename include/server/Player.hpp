#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <SFML/System.hpp>
#include <cstdlib>
#include <stack>
#include "server/Card.hpp"
#include "server/Constraints.hpp"

class Board;

/// Represents one of the two players for a given game.
class Player
{
	public:
		/// Constructor
		Player(unsigned id);

		/// Destructor.
		~Player() = default;

		/// Interface for basic gameplay (board)
		void turnEnter(unsigned turn);
		void turnLeave(unsigned turn);

		void cardPick(unsigned amount=0);  /// Move the card on top of the deck to the player's hand
		void cardUse(unsigned handIndex);  /// Use the card
		void cardAttack(unsigned boardIndex, unsigned victim);  /// Attack victim with the card

		/// Effects
		void setConstraint(std::vector<unsigned> args);
		// void loseHandCards(std::vector<unsigned> args);
		void damageBoardCreatures(std::vector<unsigned> args);


	private:
		void cardDiscardFromHand(unsigned handIndex);  /// Move the card at handIndex from the player's hand to the bin
		void cardDiscardFromBoard(unsigned boardIndex);  /// Move the card at boardIndex from the board to the bin

		std::stack<Card *> _cardDeck;  ///< Cards that are in the deck (not usable yet)
		std::vector<Card *> _cardHand;  ///< Cards that are in the player's hand (usable)
		std::vector<Card *> _cardBoard;  ///< Cards that are on the board (usable for attacks)
		std::vector<Card *> _cardBin;  ///< Cards that are discarded (dead creatures, used spells)

		unsigned _energyPoints;
		unsigned _lifePoints;

		Board* _board;
		unsigned _id;

		std::vector<std::pair<unsigned, unsigned>> _constraintsArray [constraintCount];
		unsigned getConstraint(unsigned constraintID);
		void setConstraint(unsigned constraintID, unsigned value, unsigned turns);

		struct
		{
			unsigned cardsPicked=0;
			unsigned cardsUsed=0;
			unsigned creaturesPlaced=0;
			unsigned spellsUsed=0;
		} _turnData;
};


#endif// _PLAYER_HPP
