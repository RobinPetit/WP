#ifndef _CARD_DATA_CLIENT_HPP_
#define _CARD_DATA_CLIENT_HPP_

// WizardPocker
#include "common/CardData.hpp"    // CommonCreatureData,... CostValue,...
// std-C++
#include <string>

/*/// CardData class: hold data of a card template (i.e. a card out-game)
class CardData : public CommonCardData
{
protected:
	std::string _name;
	std::string _description;

public:
	/// Constructor
	CardData(cardId, const std::string& name, CostValue, const std::string& description);

	/// Getters
	inline const std::string& getName() const;
	inline const std::string& getDescription() const;

	virtual ~CardData() = default;
};*/


/// CreatureData class: hold data of a creature card template (i.e. a card out-game)
class CreatureData : public CommonCreatureData
{
private:
	std::string _name; // To avoid a multiple inheritance, prohibited by INFOF204
	std::string _description;

public:
	/// Constructor
	CreatureData(cardId, const std::string& name, CostValue, const std::string& description,
	             AttackValue, HealthValue, ShieldValue, ShieldType);

	/// Getters
	inline const std::string& getName() const;
	inline const std::string& getDescription() const;

	virtual ~CreatureData() = default;
};


/// SpellData class: hold data of a spell card template (i.e. a card out-game)
class SpellData : public CommonSpellData
{
private:
	std::string _name; // To avoid a multiple inheritance, prohibited by INFOF204
	std::string _description;

public:
	/// Constructor
	SpellData(cardId, const std::string& name, CostValue, const std::string& description);

	/// Getters
	inline const std::string& getName() const;
	inline const std::string& getDescription() const;

	virtual ~SpellData() = default;
};

#endif  // _CARD_DATA_CLIENT_HPP
