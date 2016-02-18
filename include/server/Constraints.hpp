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
constexpr unsigned P_CONSTRAINTS_COUNT = 10; //total number of constraints
enum PLAYER_CONSTRAINTS : unsigned
{
	P_CARD_PICK_AMOUNT, 			//amount of cards to pick each turn
	P_START_ENERGY_POINTS,			//initial amount of energy points each turn
	P_ADDED_LIFE_POINTS,			//amount of life points given each turn
	P_SUBED_LIFE_POINTS, 			//amount of life points taken each turn
	P_SUBED_LIFE_POINTS_DECK_EMPTY,	//amount of life points taken each turn when deck is empty
	P_USE_CARD_LIMIT,				//limit for using cards
	P_CALL_SPELL_LIMIT,				//limit for calling spells
	P_ATTACK_WITH_CREATURE_LIMIT,	//limit for attacking with creatures
	P_PLACE_CREATURE_LIMIT,			//limit for placing creatures on board
	P_CREATURES_ON_BOARD_LIMIT		//limit for number of creatures on the board
};

#endif  // _CONSTRAINTS_HPP_
