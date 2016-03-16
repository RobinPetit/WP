#include "client/ClientCardData.hpp"

// ClientCreatureData
ClientCreatureData::ClientCreatureData(cardId id, const std::string& name, CostValue cost, const std::string& description,
                           AttackValue attack, HealthValue health, ShieldValue shield, ShieldType shieldType) :
	CommonCreatureData(id, cost, attack, health, shield, shieldType),
	_name(name), _description(description)
{
}

const std::string& ClientCreatureData::getName() const
{
	return _name;
}

const std::string& ClientCreatureData::getDescription() const
{
	return _description;
}


// SpellCard
ClientSpellData::ClientSpellData(cardId id, const std::string& name, CostValue cost, const std::string& description) :
	CommonSpellData(id, cost),
	_name(name), _description(description)
{
}

const std::string& ClientSpellData::getName() const
{
	return _name;
}

const std::string& ClientSpellData::getDescription() const
{
	return _description;
}
