#ifndef _CLIENT_CARD_CLIENT_HPP
#define _CLIENT_CARD_CLIENT_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/CardData.hpp"
#include "common/Identifiers.hpp"

class ClientCard
{
private:
	cardId _cardId;
	std::string _name;
	int _cost;
	std::string _description;

public:
	/// Constructor
	ClientCard(cardId cardIdentifier, const std::string& name, int cost, const std::string& description);

	/// Getters
	cardId getID() const;
	const std::string& getName() const;
	int getEnergyCost() const;
	const std::string& getDescription() const;

	/// Methods
	virtual inline bool isCreature() const = 0;
	virtual inline bool isSpell() const = 0;

	virtual ~ClientCard() = default;
};

#endif  // _CLIENT_CARD_CLIENT_HPP
