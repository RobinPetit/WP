#include "client/ClientCard.hpp"

ClientCard::Card(cardId cardIdentifier, const std::string& name, int cost, const std::string& description) :
	_cardId(cardIdentifier), _name(name), _cost(cost), _description(description) {}

cardId ClientCard::getID() const
{
	return _cardId;
}

const std::string& ClientCard::getName() const
{
	return _name;
}

int ClientCard::getEnergyCost() const
{
	return _cost;
}

const std::string& ClientCard::getDescription() const
{
	return _description;
}
