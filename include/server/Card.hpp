#ifndef _CARD_SERVER_HPP
#define _CARD_SERVER_HPP

// std-C++ headers
#include <string>
// WizardPoker headers
#include "server/Constraints.hpp"
#include "server/CardData.hpp"

class Card
{
private:
	int _cost;
	int _cardID;  //Will be needed for communicating modification to player's hand/deck/...
	std::vector<EffectParamsCollection> _effects;	//Effects

public:
	/// Constructor
	Card(int cost=0, std::vector<EffectParamsCollection> effects = {{}});

	/// Getters
	int getCost();
	std::vector<EffectParamsCollection> getEffects();


	/// Methods
	virtual bool isCreature()=0;
	virtual bool isSpell()=0;

	virtual ~Card()=default;

};

#endif //_CARD_SERVER_HPP
