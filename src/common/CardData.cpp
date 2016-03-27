// WizardPoker headers
#include "common/GameData.hpp"
#include "common/CardData.hpp"

// CommonCardData base class
CommonCardData::CommonCardData(CardId id, int cost) :
	_id(id),
	_cost(cost)
{
}

CardId CommonCardData::getId() const
{
	return _id;
}

int CommonCardData::getCost() const
{
	return _cost;
}


// CommonCreatureData
CommonCreatureData::CommonCreatureData(CardId id, int cost,
                           int attack, int health, int shield, int shieldType) :
	CommonCardData(id, cost),
	_attack(attack),
	_health(health),
	_shield(shield),
	_shieldType(shieldType)
{
}

int CommonCreatureData::getAttack() const
{
	return _attack;
}

int CommonCreatureData::getHealth() const
{
	return _health;
}

int CommonCreatureData::getShield() const
{
	return _shield;
}

int CommonCreatureData::getShieldType() const
{
	return _shieldType;
}

bool CommonCreatureData::isCreature() const
{
	return true;
}

bool CommonCreatureData::isSpell() const
{
	return false;
}


// CommonSpellData
CommonSpellData::CommonSpellData(CardId id, int cost) :
	CommonCardData(id,cost)
{
}

bool CommonSpellData::isCreature() const
{
	return false;
}

bool CommonSpellData::isSpell() const
{
	return true;
}

constexpr std::array<const char *, 4> BoardCreatureData::shieldTypes;

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

int EffectArgs::remainingArgs() const
{
	return static_cast<int>(_args.size() - _index);
}

std::string EffectArgs::toString() const
{
	std::stringstream ss;
	ss << "[";
	for(size_t i = _index; i < _args.size(); ++i)
		ss << _args.at(i) << (i+1 == _args.size() ? "" : ", ");
	ss << "]";
	return ss.str();
}
