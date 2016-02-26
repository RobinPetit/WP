#include "server/Spell.hpp"

Spell::Spell(int cost, std::vector<EffectParamsCollection> effects):
	Card(cost, effects)
{

}

bool Spell::isCreature()
{
	return false;
}

bool Spell::isSpell()
{
	return true;
}
