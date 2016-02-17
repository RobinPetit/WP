#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

// Effects
constexpr struct
{
	unsigned setConstraint = 0;			//set a constraint for the player
	unsigned loseHandCards = 1;			//lose an amount of random cards from the hand
	unsigned damageBoardCreatures = 2;	//damage all the player's creatures on the board
} effectIDs;

//Constraints
constexpr unsigned constraintCount = 2; //total number of constraints

constexpr struct
{
	unsigned cardPickAmount = 0; 	//amount of cards to pick each turn
	unsigned attackBackfire = 1; 	//number of attacks that will backfire
} constraintIDs;

#endif  // _CONSTRAINTS_HPP_
