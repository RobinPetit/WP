#include "client/CardData.hpp"

/*// CardData base class
CardData::CardData(cardId id, std::string name, CostValue cost, std::string description) :
	CommonCardData(id, cost),
	_name(name), _description(description)
{
}

const std::string& CardData::getName const
{
	return _name;
}

const std::string& CardData::getDescription const
{
	return _description;
}*/


// CreatureData
CreatureData::CreatureData(cardId id, const std::string& name, CostValue cost, const std::string& description,
                           AttackValue attack, HealthValue health, ShieldValue shield, ShieldType shieldType) :
	CommonCreatureData(id, cost, attack, health, shield, shieldType),
	_name(name), _description(description)
{
}

const std::string& CreatureData::getName() const
{
	return _name;
}

const std::string& CreatureData::getDescription() const
{
	return _description;
}


// SpellCard
SpellData::SpellData(cardId id, const std::string& name, CostValue cost, const std::string& description) :
	CommonSpellData(id, cost),
	_name(name), _description(description)
{
}

const std::string& SpellData::getName() const
{
	return _name;
}

const std::string& SpellData::getDescription() const
{
	return _description;
}
