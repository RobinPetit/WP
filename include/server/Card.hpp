#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "server/Constraints.hpp"
#include "common/CardData.hpp"
#include "common/Identifiers.hpp"

class Card
{
private:
	int _cost;
	cardId _cardId;  //Will be needed for communicating modification to player's hand/deck/...
	std::vector<EffectParamsCollection> _effects;	//Effects

public:
	/// Constructor
	Card(cardId cardIdentifier, int cost, const std::vector<EffectParamsCollection>& effects);

	/// Getters
	int getEnergyCost();
	const std::vector<EffectParamsCollection>& getEffects();
	cardId getId();


	/// Methods
	virtual bool isCreature()=0;
	virtual bool isSpell()=0;

	virtual ~Card()=default;

};

#endif //_CARD_SERVER_HPP
