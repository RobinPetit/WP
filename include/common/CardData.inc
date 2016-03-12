// This file is the part of CardData.hpp which is used to
// initialize database at compile time
// So it must follow some rules
// In particular
//  - Only C++ comments (//, not /**/)
//  - ' = 0,' after the first (and only the first) item
//  - No comma (',') after last item
//  - One or more empty line after last enum (not required for now but maybe later)
//  - enum NAME : int32_t (<space><colon><space>int32_t<newline>)

/// Subject of the effect
enum EFFECT_SUBJECTS : int32_t
{
    PLAYER_SELF = 0,
    PLAYER_OPPO,

    CREATURE_SELF_THIS,
    CREATURE_SELF_INDX,
    CREATURE_SELF_RAND,
    CREATURE_SELF_TEAM,

    CREATURE_OPPO_INDX,
    CREATURE_OPPO_RAND,
    CREATURE_OPPO_TEAM
};

/// Player effects
enum PLAYER_EFFECTS : int32_t
{
    //constraints
    PE_SET_CONSTRAINT = 0,
    //other effects
    PE_PICK_DECK_CARDS,
    PE_LOSE_HAND_CARDS,
    PE_REVIVE_BIN_CARD,
    PE_STEAL_HAND_CARD,
    PE_EXCHG_HAND_CARD,
    PE_SET_ENERGY,
    PE_CHANGE_ENERGY,
    PE_CHANGE_HEALTH,
    //count
    P_EFFECTS_COUNT
};

/// Player constraints
enum PLAYER_CONSTRAINTS : int32_t
{
    //turn-by-turn
    PC_TURN_CARDS_PICKED = 0, 			//amount of cards to pick each turn
    PC_TURN_ENERGY_INIT_CHANGE,		//amount of points added to init value each turn
    PC_TURN_ENERGY_CHANGE,			//amount of energy points added each turn
    PC_TURN_HEALTH_CHANGE,			//amount of health points added each turn
    PC_TURN_HEALTH_CHANGE_DECK_EMPTY,	//amount of health points taken each turn when deck is empty
    //passive
    PC_TEMP_CARD_USE_LIMIT,				//limit for using cards
    PC_TEMP_SPELL_CALL_LIMIT,			//limit for calling spells
    PC_TEMP_CREATURE_ATTACK_LIMIT,		//limit for attacking with creatures
    PC_TEMP_CREATURE_PLACING_LIMIT,		//limit for placing creatures on board
    PC_TEMP_CREATURE_BOARD_LIMIT,		//limit for number of creatures on the board
    //count
    P_CONSTRAINTS_COUNT
};

/// Creature effects
enum CREATURE_EFFECTS : int32_t
{
    //constraints
    CE_SET_CONSTRAINT = 0,
    //other effects
    CE_RESET_ATTACK,
    CE_RESET_HEALTH,
    CE_RESET_SHIELD,
    CE_CHANGE_ATTACK,
    CE_CHANGE_HEALTH,
    CE_CHANGE_SHIELD,
    //count
    C_EFFECTS_COUNT
};

/// Creature constraints
enum CREATURE_CONSTRAINTS : int32_t
{
    //turn-by-turn
    CC_TURN_ATTACK_CHANGE = 0,			//Attack points added each turn
    CC_TURN_HEALTH_CHANGE,			//Health points added each turn
    CC_TURN_SHIELD_CHANGE,			//Shield points added each turn
    //passive
    CC_TEMP_FORCE_ATTACKS,			// Forces the next X attacks
    CC_TEMP_BLOCK_ATTACKS,			// Block the next X attacks
    CC_TEMP_MIRROR_ATTACKS,			// Return the next X attacks to sender
    CC_TEMP_BACKFIRE_ATTACKS,		// The creature attacks turn against itself
    CC_TEMP_DISABLE_ATTACKS,		// The creature can not attack
    CC_TEMP_IS_PARALYZED,			// The creature can not be used, its effects are ignored
    //on creature death
    CC_DEATH_ATTACK_CHANGE,
    CC_DEATH_HEALTH_CHANGE,
    CC_DEATH_SHIELD_CHANGE,
    //count
    C_CONSTRAINTS_COUNT
};

enum CONSTRAINT_CONDITIONS : int32_t
{
    NO_CASTER_NEEDED = 0,	//Constraint does not care about the caster's state
    IF_CASTER_ALIVE		//Constraint applies only if CREATURE who cast it is alive AND not paralyzed
};

/// Types used for card creation:
enum CostValue : int32_t
{
    COST_0 = 0, COST_1, COST_2, COST_3, COST_4, COST_5, COST_6, COST_7, COST_8, COST_9, COST_10
};

enum AttackValue : int32_t
{
    ATTACK_0 = 0, ATTACK_1, ATTACK_2, ATTACK_3, ATTACK_4, ATTACK_5, ATTACK_6, ATTACK_7, ATTACK_8, ATTACK_9, ATTACK_10
};

enum HealthValue : int32_t
{
    HEALTH_0 = 0, HEALTH_1, HEALTH_2, HEALTH_3, HEALTH_4, HEALTH_5, HEALTH_6, HEALTH_7, HEALTH_8, HEALTH_9,
    HEALTH_10, HEALTH_11, HEALTH_12, HEALTH_13, HEALTH_14, HEALTH_15, HEALTH_16, HEALTH_17, HEALTH_18, HEALTH_19, HEALTH_20
};

enum ShieldValue : int32_t
{
    SHIELD_0 = 0, SHIELD_1, SHIELD_2, SHIELD_3, SHIELD_4, SHIELD_5
};

enum ShieldType : int32_t
{
    SHIELD_NONE = 0,		//no shield
    SHIELD_BLUE,		//allows part of the attack to deal damage if attack is stronger than shield
    SHIELD_ORANGE,		//allows all the attack to deal damage if attack is stronger than shield
    SHIELD_LEGENDARY	//creature attacks don't go through
};
