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
	unsigned cardPickAmount = 0; 		//amount of cards to pick each turn
	unsigned startEnergyPoints = 1; 	//initial amount of energy points each turn
	unsigned addLifePoints = 2;			//amount of life points given each turn
	unsigned subLifePoints = 3;			//amount of life points taken each turn
	unsigned emptyDeckSubLifePoints=4;	//amount of life points taken each turn when deck is empty
	unsigned useCardLimit=5;			//limit for using cards
	unsigned callSpellLimit=6;			//limit for calling spells
	unsigned attackWithCreatureLimit=7;	//limit for attacking with creatures
	unsigned placeCreatureLimit=8;		//limit for placing creatures on board
	unsigned creaturesOnBoardLimit=9;	//limit for number of creatures on the board
} constraintIDs;

#endif  // _CONSTRAINTS_HPP_
