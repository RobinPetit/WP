#ifndef _CARD_COMMON_HPP
#define _CARD_COMMON_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "common/CardData.hpp"


/// Hold the state an actual card
/// Should be pure abstract but destructor is needed so I havent method where put =0
class Card
{
protected:
	const CommonCardData& _prototype;
	int _cost;

public:
	/// Constructor
	explicit Card(const CommonCardData&);

	/// Getters
	int getEnergyCost() const;
	CardId getId() const;

	/// Methods
	bool isCreature() const;
	bool isSpell() const;

	virtual ~Card() = default;
};

#endif //_CARD_COMMON_HPP
