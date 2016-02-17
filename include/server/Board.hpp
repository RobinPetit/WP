#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <stack>
#include <SFML/System.hpp>
#include <Player.hpp>

//Forward declarations
class Card;

//Effects
struct {
	unsigned setConstraint=0;			//set a constraint for the player
	unsigned loseHandCards=1;			//lose an amount of random cards from the hand
	unsigned damageBoardCreatures=2;	//damage all the player's creatures on the board
} effectIDs;

//Constraints
constexpr unsigned constraintCount=2; //total number of constraints

constexpr struct {
	unsigned cardPickAmount = 0; 	//amount of cards to pick each turn
	unsigned attackBackfire = 1; 	//number of attacks that will backfire
} constraintIDs;


/// Manages one game with two players
class Board
{
	public:
		/// Destructor.
		~Board() = default;

		/// Interface for user
		void endTurn();
		void useCard(unsigned handIndex);
		void attackWithCard(unsigned boardIndex, unsigned victim);
		void quitGame(); //TODO: need identifier

		/// Interface for Card class
		void applyEffect(unsigned player, unsigned effectID, std::vector<unsigned> effectArgs);

		/// Interface for Player class
		unsigned getDefaultConstraint(unsigned constraintID);

	private:
		unsigned _turn = 0;
		Player* _activePlayer, _passivePlayer;

		//Effects
		void* [] _effects = {
			Player::*setConstraint()
			Player::*loseHandCards(),
			Player::*damageBoardCreatures()
		};

		//Constraints
		unsigned [] _constraintDefaults ={
			1,	//pick one card each turn
			0	//no attack will backfire
		};

};


#endif// _BOARD_HPP
