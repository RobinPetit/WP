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
constexpr unsigned CONSTRAINTS_COUNT = 10; //total number of constraints

enum Constraints : unsigned
{
	CARD_PICK_AMOUNT, 			//amount of cards to pick each turn
	START_ENERGY_POINTS,		//initial amount of energy points each turn
	ADDED_LIFE_POINTS,			//amount of life points given each turn
	SUBED_LIFE_POINTS, 			//amount of life points taken each turn
	SUBED_LIFE_POINTS_DECK_EMPTY,	//amount of life points taken each turn when deck is empty
	USE_CARD_LIMIT,				//limit for using cards
	CALL_SPELL_LIMIT,			//limit for calling spells
	ATTACK_WITH_CREATURE_LIMIT,//limit for attacking with creatures
	PLACE_CREATURE_LIMIT,		//limit for placing creatures on board
	CREATURES_ON_BOARD_LIMIT	//limit for number of creatures on the board
};

#endif  // _CONSTRAINTS_HPP_
