#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/CardData.hpp"

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
	cardId getId() const;

	/// Methods
	bool isCreature() const;
	bool isSpell() const;

	virtual ~Card() = 0; // To have it pure virtual
};

#endif //_CARD_COMMON_HPP
