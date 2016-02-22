#include "server/CardData.hpp"

///CREATURES
const CreatureData ALL_CREATURES[] = {
	{
		"Stanislas",
		COST_2, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
		"Tower guard",
		COST_3, ATTACK_2, HEALTH_8, SHIELD_1, SHIELD_BLUE,
		{{CREATURE_SELF,CE_SET_CONSTRAINT,CC_SELF_HEALTH_GAIN,1}},
		"This creature gains 1 HP each turn."
	},
	{
        	"Diabolic imp",
		COST_2, ATTACK_3, HEALTH_2, SHIELD_0, SHIELD_NONE,
		{{CREATURE_SELF,CE_SET_CONSTRAINT,CC_SELF_ATTACK_GAIN,1}},
		"This creature gains 1 ATTACK each turn."
	},
	{
        	"Groot",
		COST_5, ATTACK_4, HEALTH_7, SHIELD_2, SHIELD_BLUE,
		{{}},
		"We are groot."
	},
	{
        	"Little Ragnaros",
		COST_4, ATTACK_4, HEALTH_4, SHIELD_1, SHIELD_LEGENDARY,
		{{PLAYER_OPPO,PE_SET_CONSTRAINT,PC_HEALTH_POINTS_LOSS,2}},
		"Each turn, the opposite player loses 2 HP."
	},
	{
        	"White wizard",
		COST_7, ATTACK_7, HEALTH_7, SHIELD_0, SHIELD_NONE,
		{{CREATURE_SELF,CE_SET_CONSTRAINT,CC_TEAM_BLOCK_ATTACKS,2}},
		"Blocks the next 2 attacks on his team."
	},
	{
 		"Greedy imp",
		COST_2, ATTACK_2, HEALTH_2, SHIELD_2, SHIELD_ORANGE,
		{{PLAYER_SELF,PC_CARD_PICK_AMOUNT,1}},
		"The player draws another card at the beginning of his turn."
	},
	{
        	"Old witch",
		COST_5, ATTACK_6, HEALTH_5, SHIELD_1, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
        	"Warrior apprentice",
		COST_1, ATTACK_2, HEALTH_2, SHIELD_0, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
        	"Unbridled bear",
		COST_3, ATTACK_3, HEALTH_4, SHIELD_0, SHIELD_NONE,
		{{CREATURE_ALL_OPPO,CE_SUB_SHIELD,1}},
		"Takes away 1 shield of each opposite creatures."
	},
	{
        	"Giant soldier",
		COST_9, ATTACK_8, HEALTH_8, SHIELD_3, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
        	"White dragon",
		COST_10, ATTACK_9, HEALTH_10, SHIELD_0, SHIELD_NONE,
		{{CE_SET_CONSTRAINT,CREATURE_TEAM,CC_TEAM_ATTACK_GAIN,1}},
		"All your friendly creatures gain 1 ATTACK each turn."
	},
	{
        	"Resurrected triceratops",
		COST_0, ATTACK_0, HEALTH_1, SHIELD_0, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
        	"Mecha spider",
		COST_3, ATTACK_4, HEALTH_2, SHIELD_1, SHIELD_ORANGE,
		{{CREATURE_ONE_OPPO,CE_RESET_ATTACK,1}},
		"Reset the attack of one opposite creature."
	},
	{
        	"Armored leprechaun",
		COST_1, ATTACK_1, HEALTH_3, SHIELD_0, SHIELD_NONE,
		{{PE_SET_CONSTRAINT,PLAYER_SELF,PC_CARD_PICK_AMOUNT,1}},
		"The player draws another card at the beginning of his turn."
	},
	{
        	"Mirage of happiness",
		COST_3, ATTACK_0, HEALTH_5, SHIELD_3, SHIELD_BLUE,
		{{PE_SET_CONSTRAINT,PLAYER_OPPO,PC_CALL_SPELL_LIMIT,2}},
		"The opposite player can't play more than 2 spells per turn."
	},
	{
        	"Deathwing",
		COST_5, ATTACK_9, HEALTH_9, SHIELD_2, SHIELD_LEGENDARY,
		{{PE_SET_CONSTRAINT,PLAYER_SELF,PC_USE_CARD_LIMIT,0}},
		"You can't play any card while this creature is alive."
	},
	{
        	"Silver wolf",
		COST_4, ATTACK_6, HEALTH_5, SHIELD_0, SHIELD_NONE,
		{{}},
		"No effect."
	},
	{
        	"Kuriboh",
		COST_1, ATTACK_2, HEALTH_2, SHIELD_0, SHIELD_NONE,
		{{CREATURE_TEAM,CE_RESET_SHIELD,2}},
		"Resets 2 friendly shields."
	},
	{
        	"Twin-Headed dog",
		COST_4, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_ORANGE,
		{{CE_SET_CONSTRAINT,CREATURE_SELF,CE_ADD_ATTACK,1},{CE_SET_CONSTRAINT,CREATURE_SELF,CE_ADD_HEALTH,1}},
		"Gains 1 HP and 1 ATTACK each turn."
	},
	{
        	"Humanoide maid",
		COST_2, ATTACK_0, HEALTH_5, SHIELD_0, SHIELD_NONE,
		{{CE_SET_CONSTRAINT,CREATURE_TEAM,CE_ADD_HEALTH,2},{PE_SET_CONSTRAINT,PLAYER_SELF,CE_ADD_HEALTH,2}},
		"Gives a friendly creature and this creature owner 2 HP each turn."
	}
};

///SPELLS
const SpellData ALL_SPELLS[] = {
	{
		"Abracadabra",
		COST_10,
		{{PLAYER_OPPO, PE_LOSE_HAND_CARDS, 2}},
		"Your opponent loses 2 cards from his hand"
	},
	{
		"Block",
		COST_9,
		{{CREATURE_TEAM, CC_TEAM_BLOCK_ATTACKS, 3}},
		"Block the next 3 attacks"
	},
	{
		"Care",
		COST_5,
		{{CREATURE_SELF, CE_ADD_HEALTH, 2}},
		"Increase a creature's health of 2 points"
	},
	{
		"Complete Care",
		COST_7,
		{{CREATURE_SELF, CE_RESET_HEALTH, 0}},
		"Restore the health point of a creature"
	},
	{
		"Discard",
		COST_9,
		{{PLAYER_OPPO, PE_LOSE_HAND_CARDS, 1}},
		"Force the opponent to discard a card of his choice"
	},
	{
		"Epidemic",
		COST_8,
		{{CREATURE_ALL_OPPO, CE_SUB_ATTACK, 1}},
		"Decrease all opponent's creatures' attack of 1 point"
	},
	{
		"Exchange",
		COST_7,
		{{PLAYER_OPPO, PE_EXCHG_HAND_CARD, 1}},
		"Each player chose an opponent's card and trade them"
	},
	/* TODO: CE_SELF_HYPNOTIZED NOT CREATED YET {
		"Hypnosis",
		COST_4,
		{{CREATURE_ONE_OPPO, CE_SELF_HYPNOTIZED, 3}},
		"Take control of an ennemy creature for 3 turn"
	},*/
	{
		"Metamorphosis",
		COST_4,
		{{CREATURE_SELF, 0/*Effect missing*/, 1}},
		"Transforms 1 creature in another random creature"
	},
	{
		"Mirror",
		COST_6,
		{{CREATURE_ONE_OPPO, 0/*Effect missing*/, 0}},
		"Send back the next opponent's attack"
	},
	{
		"Paralyse",
		COST_3,
		{{CREATURE_ONE_OPPO, CC_SELF_BLOCK_ATTACKS, 2}},
		"Forbid the opponent to use a card for 2 turns"
	},
	{
		"Poison",
		COST_4,
		{{CREATURE_ONE_OPPO, CE_SUB_HEALTH, 2}},
		"Decrease an opponent's creature's health of 2 points"
	},
	{
		"Picking",
		COST_3,
		{{PLAYER_SELF, PE_PICK_DECK_CARDS, 1}},
		"Picking 1 more cards from the deck"
	},
	{
		"Resurrection",
		COST_9,
		{{CREATURE_SELF, PE_REVIVE_BIN_CARD, 1}},
		"Allow the player to get 1 discarded card"
	},
	{
		"Shield",
		COST_2,
		{{CREATURE_TEAM, CC_SELF_BLOCK_ATTACKS, 1}},
		"Absord the next opponent's attack"
	},
	{
		"Sickness",
		COST_7,
		{{CREATURE_ONE_OPPO, CE_SUB_ATTACK, 2}},
		"Decrease an opponent's creature's attack of 2 points"
	},
	{
		"Stealing",
		COST_8,
		{{PLAYER_OPPO, PE_STEAL_HAND_CARD, 1}},
		"Take 1 random card from the opponent's deck"
	},
	/* TODO: STICKY NOT DONE YET {
		"Sticky",
		COST_7,	//~I put a random cost, as it was not specified in CSV file (NBA)
		{{CREATURE_ALL_OPPO, C_STICKY, 2}},
		"Your opponent's spell cost 2 more next Turn"
	},*/
	{
		"Team Care",
		COST_6,
		{{CREATURE_TEAM, CE_ADD_HEALTH, 2}},
		"Increase team's creatures' health of 2 points"
	},
	/* TODO: TRAP NOT DONE YET {
		"Trap",
		COST_2,
		{{CREATURE_ONE_OPPO, C_TRAP, 3}},
		"Deal 3 damage or summon two 1/1 snacks"
	}*/
};
