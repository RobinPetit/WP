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
constexpr unsigned constraintCount = 10; //total number of constraints

constexpr enum : unsigned
{
	cardPickAmount=0, 		//amount of cards to pick each turn
	startEnergyPoints,		//initial amount of energy points each turn
	addLifePoints,			//amount of life points given each turn
	subLifePoints, 			//amount of life points taken each turn
	emptyDeckSubLifePoints,	//amount of life points taken each turn when deck is empty
	useCardLimit,			//limit for using cards
	callSpellLimit,			//limit for calling spells
	attackWithCreatureLimit,//limit for attacking with creatures
	placeCreatureLimit,		//limit for placing creatures on board
	creaturesOnBoardLimit	//limit for number of creatures on the board
} Constraint;

#endif  // _CONSTRAINTS_HPP_
