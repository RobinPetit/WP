#include "server/Spell.hpp"

Spell::Spell(cardId cardIdentifier, int cost, const std::vector<EffectParamsCollection>& effects):
	Card(cardIdentifier, cost, effects)
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
