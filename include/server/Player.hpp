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

		/// Effects
		void setConstraint(std::vector<unsigned> args);
		void pickSomeCards(std::vector<unsigned> args);
		void loseHandCards(std::vector<unsigned> args);
        void stealHandCard(std::vector<unsigned> args);
        void exchangeHandCard(std::vector<unsigned> args);

        void addEnergyPoints(std::vector<unsigned> args);
        void subEnergyPoints(std::vector<unsigned> args);
		void addLifePoints(std::vector<unsigned> args);
		void subLifePoints(std::vector<unsigned> args);


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
		unsigned _lifePoints;

		void setEnergyPoints(unsigned points);

		Board* _board;
		Player* _opponent = nullptr;
		unsigned _id;

		//Constraints
		unsigned _constraintDefaults[P_CONSTRAINTS_COUNT] =
		{
			//TURN-BASED CONSTRAINTS
			1,		//pick one card
			10,		//energy points start at 10
			0, 		//sub 0 life points
			0,		//add 0 life points
			5, 		//if deck empty, sub 5 life points
			//PASSIVE CONSTRAINTS
			100,	//no limit for using cards
			100,	//no limit for calling spells
			100, 	//no limit for attacking with creatures
			6,		//arbitrary limit for placing creatures (TODO: discuss)
			6		//arbitrary limit for amount of creatures on board
		};

		std::vector<std::pair<unsigned, unsigned>> _constraintsArray [P_CONSTRAINTS_COUNT];
		unsigned getConstraint(unsigned constraintID);
		void setConstraint(unsigned constraintID, unsigned value, unsigned turns);
		void timeOutConstraints();

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
