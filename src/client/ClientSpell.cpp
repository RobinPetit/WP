#include "client/ClientSpell.hpp"

ClientSpell::ClientSpell(const ClientSpellData& cardData):
	Card(cardData)
{
}

/*--------------------------- PRIVATE METHOD */
inline const ClientSpellData& ClientSpell::prototype() const
{
	return static_cast<const ClientSpellData&>(_prototype);
}

const std::string& ClientSpell::getName()
{
	return prototype().getName();
}

const std::string& ClientSpell::getDescription()
{
	return prototype().getDescription();
}
