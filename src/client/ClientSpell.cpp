#include "client/ClientSpell.hpp"

ClientSpell::ClientSpell(cardId cardIdentifier, const std::string& name, int cost, const std::string& description) :
	ClientCard(cardIdentifier, name, cost, description) {}

bool ClientSpell::isCreature()
{
	return false;
}

bool ClientSpell::isSpell()
{
	return true;
}
