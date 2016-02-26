#include "server/Spell.hpp"

Spell::Spell(int cardID, int cost, std::vector<EffectParamsCollection> effects):
	Card(cardID, cost, effects)
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
