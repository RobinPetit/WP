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
constexpr unsigned P_CONSTRAINTS_COUNT = 10; //number of player constraints
enum PLAYER_CONSTRAINTS : unsigned
{
	//turn-by-turn
	P_CARD_PICK_AMOUNT, 			//amount of cards to pick each turn
	P_ENERGY_POINTS_VALUE,			//initial amount of energy points each turn
	P_LIFE_POINTS_GAIN,				//amount of life points given each turn
	P_LIFE_POINTS_LOSS, 			//amount of life points taken each turn
	P_LIFE_POINTS_LOSS_DECK_EMPTY,	//amount of life points taken each turn when deck is empty
	//passive
	P_USE_CARD_LIMIT,				//limit for using cards
	P_CALL_SPELL_LIMIT,				//limit for calling spells
	P_ATTACK_WITH_CREATURE_LIMIT,	//limit for attacking with creatures
	P_PLACE_CREATURE_LIMIT,			//limit for placing creatures on board
	P_CREATURES_ON_BOARD_LIMIT		//limit for number of creatures on the board
};

//I MAY use this
constexpr unsigned C_CONSTRAINTS_COUNT = 1; //number of creature constraints
enum CREATURE_CONSTRAINTS : unsigned
{
    //turn-by-turn
    C_SELF_LIFE_POINTS_GAIN,
    C_ALL_LIFE_POINTS_GAIN,
    C_SELF_ATTACK_POINTS_GAIN,
    C_ALL_ATTACK_POINTS_GAIN,
    C_SHIELD_POINTS_LOSS
    //more to come
};

#endif  // _CONSTRAINTS_HPP_
