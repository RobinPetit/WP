#ifndef _CARD_DATA_SEVER_HPP_
#define _CARD_DATA_SEVER_HPP_

// WizardPocker
#include "common/CardData.hpp" // CommonCreatureData,... CostValue,...
// std-C++
#include <vector>

/// CreatureData class: hold data of a creature card template (i.e. a card out-game)
class CreatureData : public CommonCreatureData
{
private:
	std::vector<EffectParamsCollection> _effects; // To avoid a multiple inheritance, prohibited by INFOF204

public:
	/// Constructor
	CreatureData(cardId, CostValue, const std::vector<EffectParamsCollection>& effects,
	             AttackValue, HealthValue, ShieldValue, ShieldType);

	/// Getters
	const std::vector<EffectParamsCollection>& getEffects() const;

	virtual ~CreatureData() = default;
};


/// SpellData class: hold data of a spell card template (i.e. a card out-game)
class SpellData : public CommonSpellData
{
private:
	std::vector<EffectParamsCollection> _effects; // To avoid a multiple inheritance, prohibited by INFOF204

public:
	/// Constructor
	SpellData(cardId, CostValue, const std::vector<EffectParamsCollection>& effects);

	/// Getters
	const std::vector<EffectParamsCollection>& getEffects() const;

	virtual ~SpellData() = default;
};



#endif  // _CARD_DATA_SERVER_HPP
