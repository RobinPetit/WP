#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "server/Constraints.hpp"
#include "server/CardData.hpp"

class Card
{
protected:
	const CommonCardData& _prototype;
	int _cost;

public:
	/// Constructor
	Card(const CommonCardData&);

	/// Getters
	int getEnergyCost() const;
	cardId getID() const;

	/// Methods
	bool isCreature() const;
	bool isSpell() const;

	virtual ~Card() = default;
};

#endif //_CARD_SERVER_HPP
