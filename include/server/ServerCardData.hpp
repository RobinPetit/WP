#ifndef _CARD_DATA_SEVER_HPP_
#define _CARD_DATA_SEVER_HPP_

// WizardPocker
#include "common/CardData.hpp" // CommonCreatureData,... CostValue,...
// std-C++
#include <vector>

/// ServerCreatureData class: hold data of a creature card template (i.e. a card out-game)
class ServerCreatureData : public CommonCreatureData
{
private:
	std::vector<EffectParamsCollection> _effects; // To avoid a multiple inheritance, prohibited by INFOF204

public:
	/// Constructor
	ServerCreatureData(CardId, int cost, const std::vector<EffectParamsCollection>& effects,
	                   int attack, int health, int shield, int shieldType);

	/// Getters
	const std::vector<EffectParamsCollection>& getEffects() const;

	virtual ~ServerCreatureData() = default;
};


/// ServerSpellData class: hold data of a spell card template (i.e. a card out-game)
class ServerSpellData : public CommonSpellData
{
private:
	std::vector<EffectParamsCollection> _effects; // To avoid a multiple inheritance, prohibited by INFOF204

public:
	/// Constructor
	ServerSpellData(CardId, int cost, const std::vector<EffectParamsCollection>& effects);

	/// Getters
	const std::vector<EffectParamsCollection>& getEffects() const;

	virtual ~ServerSpellData() = default;
};

#endif  // _CARD_DATA_SERVER_HPP
