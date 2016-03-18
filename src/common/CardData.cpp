// WizardPoker headers
#include "common/GameData.hpp"
#include "common/CardData.hpp"

constexpr std::array<const char *, 4> BoardCreatureData::shieldTypes;

///CREATURES
const CreatureData ALL_CREATURES[] = {
	{
		"Tower guard",
		COST_1, ATTACK_2, HEALTH_8, SHIELD_1, SHIELD_BLUE,
		{{CREATURE_SELF_THIS,CE_SET_CONSTRAINT,CC_TURN_HEALTH_CHANGE,1,UNLIMITED_TURNS,NO_CASTER_NEEDED}},
		"This creature gains 1 HEALTH each turn"
	},
	{
		"Diabolic imp",
		COST_1, ATTACK_3, HEALTH_2, SHIELD_1, SHIELD_NONE,
		{{CREATURE_SELF_THIS,CE_SET_CONSTRAINT,CC_TURN_ATTACK_CHANGE,1,UNLIMITED_TURNS,NO_CASTER_NEEDED}},
		"This creature gains 1 ATTACK each turn"
	},
	{
		"Little Ragnaros",
		COST_2, ATTACK_4, HEALTH_4, SHIELD_1, SHIELD_LEGENDARY,
		{{PLAYER_OPPO,PE_SET_CONSTRAINT,PC_TURN_HEALTH_CHANGE,-2,UNLIMITED_TURNS,IF_CASTER_ALIVE}},
		"Each turn, your opponent loses 2 HP"
	},
	{
		"White wizard",
		COST_3, ATTACK_7, HEALTH_7, SHIELD_1, SHIELD_NONE,
		{{CREATURE_SELF_THIS,CE_SET_CONSTRAINT,CC_TEMP_BLOCK_ATTACKS,2,UNLIMITED_TURNS,NO_CASTER_NEEDED}},
		"Blocks the next 2 attacks on your team"
	},
	{
 		"Greedy imp",
		COST_1, ATTACK_2, HEALTH_2, SHIELD_2, SHIELD_ORANGE,
		{{PLAYER_SELF,PE_SET_CONSTRAINT,PC_TURN_CARDS_PICKED,2,1,NO_CASTER_NEEDED}},
		"Allows you to draw one more card at the beginning of your next turn"
	},
	{
		"Unbridled bear",
		COST_2, ATTACK_3, HEALTH_4, SHIELD_1, SHIELD_NONE,
		{{CREATURE_OPPO_TEAM,CE_CHANGE_SHIELD,-1}},
		"Takes away 1 shield of each opposite creatures when placed"
	},
	{
		"Giant soldier",
		COST_9, ATTACK_8, HEALTH_8, SHIELD_3, SHIELD_BLUE,
		{{}},
		"No effect"
	},
	{
		"White dragon",
		COST_10, ATTACK_9, HEALTH_10, SHIELD_1, SHIELD_NONE,
		{{CREATURE_SELF_TEAM,CE_SET_CONSTRAINT,CC_TURN_ATTACK_CHANGE,1,UNLIMITED_TURNS,IF_CASTER_ALIVE}},
		"All your friendly creatures gain 1 ATTACK each turn"
	},
	{
		"Mecha spider",
		COST_1, ATTACK_4, HEALTH_2, SHIELD_1, SHIELD_ORANGE,
		{{CREATURE_OPPO_RAND,CE_RESET_ATTACK,1}},
		"Reset the attack for a random opponent's creature"
	},
	{
		"Mirage of happiness",
		COST_2, ATTACK_1, HEALTH_5, SHIELD_3, SHIELD_BLUE,
		{{PLAYER_OPPO,PE_SET_CONSTRAINT,PC_TEMP_SPELL_CALL_LIMIT,2,UNLIMITED_TURNS,NO_CASTER_NEEDED}},
		"Your opponent can't play more than 2 spells per turn."
	}/*,
	{
		"Armored leprechaun",
		COST_1, ATTACK_1, HEALTH_3, SHIELD_1, SHIELD_NONE,
		{{PLAYER_SELF,PE_SET_CONSTRAINT,PC_TURN_CARDS_PICKED,1,1,NO_CASTER_NEEDED}},
		"Allows you to draw one more card at the beginning of your next turn"
	},

	{
		"Old witch",
		COST_5, ATTACK_6, HEALTH_5, SHIELD_1, SHIELD_LEGENDARY,
		{{}},
		"No effect"
	},
	{
		"Warrior apprentice",
		COST_1, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
		"Resurrected triceratops",
		COST_0, ATTACK_0, HEALTH_1, SHIELD_1, SHIELD_NONE,
		{{}},
		"No effect"
	},
	{
		"Groot",
		COST_5, ATTACK_4, HEALTH_7, SHIELD_2, SHIELD_BLUE,
		{{}},
		"We are groot."
	},
	{
		"Deathwing",
		COST_5, ATTACK_9, HEALTH_9, SHIELD_2, SHIELD_LEGENDARY,
		{{PLAYER_SELF,PE_SET_CONSTRAINT,PC_LIMIT_CARD_USE,0,}},
		"You can't play any card while this creature is alive."
	},
	{
		"Silver wolf",
		COST_4, ATTACK_6, HEALTH_5, SHIELD_1, SHIELD_NONE,
		{{}},
		"No effect."
	},
	{
		"Kuriboh",
		COST_1, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_NONE,
		{{CREATURE_SELF_TEAM,CE_RESET_SHIELD,2}},
		"Resets 2 friendly shields."
	},
	{
		"Twin-Headed dog",
		COST_4, ATTACK_2, HEALTH_2, SHIELD_1, SHIELD_ORANGE,
		{{CREATURE_SELF_THIS,CE_SET_CONSTRAINT,CE_ADD_ATTACK,1},{CE_SET_CONSTRAINT,CREATURE_SELF,CE_ADD_HEALTH,1}},
		"Gains 1 HP and 1 ATTACK each turn."
	},
	{
		"Humanoide maid",
		COST_2, ATTACK_0, HEALTH_5, SHIELD_1, SHIELD_NONE,
		{{CE_SET_CONSTRAINT,CREATURE_TEAM,CE_ADD_HEALTH,2},{PE_SET_CONSTRAINT,PLAYER_SELF,CE_ADD_HEALTH,2}},
		"Gives a friendly creature and this creature owner 2 HP each turn."
	}*/
};

///SPELLS
const SpellData ALL_SPELLS[] = {
	{
		"Abracadabra",
		COST_10,
		{{PLAYER_OPPO, PE_LOSE_HAND_CARDS, 2}},
		"Your opponent loses 2 random cards from his hand"
	},
	{
		"Block",
		COST_9,
		{{CREATURE_SELF_TEAM,CE_SET_CONSTRAINT,CC_TEMP_BLOCK_ATTACKS,3,UNLIMITED_TURNS,NO_CASTER_NEEDED}},
		"Block the next 3 attacks for the whole team"
	},
	{
		"Care",
		COST_2,
		{{CREATURE_SELF_INDX, CE_CHANGE_HEALTH, 2}},
		"Increase a creature's health of 2 points"
	},
	{
		"Complete Care",
		COST_7,
		{{CREATURE_SELF_INDX, CE_RESET_HEALTH}},
		"Restore the health point of a creature of your choice"
	},
	{
		"Discard",
		COST_2,
		{{PLAYER_OPPO, PE_LOSE_HAND_CARDS, 1}},
		"Your opponent loses a random card from his hand"
	},
	{
		"Sickness",
		COST_1,
		{{CREATURE_OPPO_TEAM, CE_CHANGE_ATTACK, -1}},
		"Decrease all your opponent's creatures' attack of 1 point"
	},
	{
		"Epidemic",
		COST_4,
		{{CREATURE_OPPO_TEAM, CE_CHANGE_ATTACK, -2}},
		"Decrease all your opponent's creatures' attack of 2 points"
	},
	{
		"Flash heal",
		COST_1,
		{{CREATURE_SELF_RAND, CE_CHANGE_HEALTH, 1}},
		"Increase a random creature's health of 2 points"
	},
	{
		"Robbery",
		COST_2,
		{{CREATURE_OPPO_TEAM, CE_CHANGE_ATTACK, -1},{CREATURE_SELF_TEAM, CE_CHANGE_ATTACK, 1}},
		"Decrease all your opponent's creatures' attack of 1 point and increase yours of 1 point"
	},
	{
		"Grand Theft Card",
		COST_1,
		{{PLAYER_SELF, PE_STEAL_HAND_CARD}},
		"You steal a random card from the opponent's hand"
	}/*,
	// TODO: CE_SELF_HYPNOTIZED NOT CREATED YET
	{
		"Hypnosis",
		COST_4,
		{{CREATURE_ONE_OPPO, CE_SELF_HYPNOTIZED, 3}},
		"Take control of an ennemy creature for 3 turn"
	},
	{
		"Metamorphosis",
		COST_4,
		{{CREATURE_SELF, 0Effect missing, 1}},
		"Transforms 1 creature in another random creature"
	},
	{
		"Mirror",
		COST_6,
		{{CREATURE_ONE_OPPO, 0Effect missing, 0}},
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
	// TODO: STICKY NOT DONE YET
	{
		"Sticky",
		COST_7,	//~I put a random cost, as it was not specified in CSV file (NBA)
		{{CREATURE_ALL_OPPO, C_STICKY, 2}},
		"Your opponent's spell cost 2 more next Turn"
	},
	{
		"Team Care",
		COST_6,
		{{CREATURE_TEAM, CE_ADD_HEALTH, 2}},
		"Increase team's creatures' health of 2 points"
	},
	// TODO: TRAP NOT DONE YET
	{
		"Trap",
		COST_2,
		{{CREATURE_ONE_OPPO, C_TRAP, 3}},
		"Deal 3 damage or summon two 1/1 snacks"
	}*/
};

const std::size_t nbSpells{sizeof(ALL_SPELLS) / sizeof(SpellData)};

const std::size_t nbCreatures{sizeof(ALL_CREATURES) / sizeof(CreatureData)};

EffectArgs::EffectArgs(const EffectParamsCollection& args):
	_args(args),
	_index{0}
{

}

EffectArgs::EffectArgs(const std::initializer_list<int>& args):
	_args(args),
	_index{0}
{

}

int EffectArgs::getArg()
{
	return _args.at(_index++);
}

int EffectArgs::peekArg() const
{
	return _args.at(_index);
}

int EffectArgs::remainingArgs()
{
	return static_cast<int>(_args.size() - _index);
}

std::string EffectArgs::toString() const
{
	std::stringstream ss;
	ss << "[";
	for(size_t i = _index; i < _args.size(); ++i)
		ss << ((i+1 == _args.size() ? "" : ", ") + _args.at(i));
	ss << "]";
	return ss.str();
}
