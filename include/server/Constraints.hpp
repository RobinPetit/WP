#ifndef _CONSTRAINTS_HPP_
#define _CONSTRAINTS_HPP_

#include <vector>


// Effects: mettre ailleurs ?

enum SPELL_EFFECTS : unsigned
{
	S_BLOCK,			// Block the next X attacks
	S_CARE,				// Give to a monster X lifePoints
	S_EXCHANGE,			// Each player chose an opponent's card and trade them
	S-HYPNOSIS,			// Take control of an ennemy creature for X turn
	S_MIRROR,			// Send back the next opponent's attack ( trap )
	S_PARALYZE,			// Forbid the opponent to use a card for X turns
	S_RESURRECTION,			// Allow the player using this effect to get a discarded card
	S_SHIELD,			// Absord the next opponent's attack
	S_STEALING,			// Take a random card from the opponent's deck
	S_STICKY,			// Your opponent's spell cost X more next Turn
	S_TRAP				// Deal 3 damage or summon two 1/1 snacks
};


/// PLAYER CONSTRAINTS
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

constexpr unsigned P_CONSTRAINT_DEFAULTS[P_CONSTRAINTS_COUNT] =
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


/// CREATURE CONSTRAINTS
constexpr unsigned C_CONSTRAINTS_COUNT = 6; //number of creature constraints
enum CREATURE_CONSTRAINTS : unsigned
{
    //turn-by-turn
    C_SELF_LIFE_POINTS_GAIN,
    C_TEAM_LIFE_POINTS_GAIN,
    C_OPPO_LIFE_POINTS_LOSS,
    C_SELF_ATTACK_POINTS_GAIN,
    C_TEAM_ATTACK_POINTS_GAIN,
    C_SELF_SHIELD_POINTS_LOSS
    //passive to come
};

constexpr unsigned C_CONSTRAINT_DEFAULTS[C_CONSTRAINTS_COUNT] =
{
	//TURN-BASED CONSTRAINTS: all set to 0, change with effects
	0,		//
	0,		//
	0, 		//
	0,		//
	0, 		//
	0		//
	//PASSIVE CONSTRAINTS: to come (agro, taunt...)
};

/// LA CLASSE!
class ConstraintList
{
private:
	const unsigned* _defaultValues;
	const unsigned _size;
	std::vector<std::pair<unsigned, unsigned>>* _timedValues;

public:
	ConstraintList(const unsigned* defaultValues, const unsigned arraySize);
	void setConstraint(unsigned constraintID, unsigned value, unsigned turns);
	unsigned getConstraint(unsigned constraintID);
	void timeOutConstraints();
};

ConstraintList::ConstraintList(const unsigned* defaultValues, const unsigned arraySize):
	_defaultValues(defaultValues), _size(arraySize)
{
	_timedValues = new std::vector<std::pair<unsigned, unsigned>>[_size];
}

void ConstraintList::setConstraint(unsigned constraintID, unsigned value, unsigned turns)
{
	_timedValues[constraintID].push_back(std::make_pair(value, turns));
}

unsigned ConstraintList::getConstraint(unsigned constraintID)
{
    if (_timedValues[constraintID].empty())
		return _defaultValues[constraintID];
    else
		return _timedValues[constraintID].rbegin()->first; //TODO change this model ?
}

void ConstraintList::timeOutConstraints()
{
    for (unsigned i=0; i<_size; i++)
    {
        std::vector<std::pair<unsigned, unsigned>> vect = _timedValues[i];
        for (std::vector<std::pair<unsigned, unsigned>>::iterator vectIt=vect.begin(); vectIt!=vect.end();)
        {
            if (vectIt->second == 1) vectIt = vect.erase(vectIt);
            else
            {
				vectIt->second--;
				vectIt++;
            }
        }
    }
}

#endif  // _CONSTRAINTS_HPP_
