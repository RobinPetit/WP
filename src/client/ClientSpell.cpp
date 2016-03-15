#include "client/ClientSpell.hpp"

ClientSpell::ClientSpell(const SpellData& cardData):
	Card(cardData)
{
}

/*--------------------------- PRIVATE METHOD */
inline const SpellData& ClientSpell::prototype() const
{
	return static_cast<const SpellData&>(_prototype);
}

const std::string& ClientSpell::getName()
{
	return prototype().getName();
}

const std::string& ClientSpell::getDescription()
{
	return prototype().getDescription();
}
