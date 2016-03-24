#ifndef _CLIENT_CARD_DATA_CLIENT_HPP_
#define _CLIENT_CARD_DATA_CLIENT_HPP_

// WizardPocker
#include "common/CardData.hpp"  // CommonCreatureData,... CostValue,...
// std-C++
#include <string>

/// ClientCreatureData class: hold data of a creature card template (i.e. a card out-game)
class ClientCreatureData : public CommonCreatureData
{
private:
	std::string _name; // To avoid a multiple inheritance, prohibited by INFOF204
	std::string _description;

public:
	/// Constructor
	ClientCreatureData(cardId, const std::string& name, int cost, const std::string& description,
	                   int health, int attack, int shield, int shieldType);

	/// Getters
	// I don't know why I get 'W used but never defined' and 'undefined reference to' when I inline?
	const std::string& getName() const;
	const std::string& getDescription() const;

	virtual ~ClientCreatureData() = default;
};


/// ClientSpellData class: hold data of a spell card template (i.e. a card out-game)
class ClientSpellData : public CommonSpellData
{
private:
	std::string _name; // To avoid a multiple inheritance, prohibited by INFOF204
	std::string _description;

public:
	/// Constructor
	ClientSpellData(cardId, const std::string& name, int cost, const std::string& description);

	/// Getters
	// I don't know why I get 'W used but never defined' and 'undefined reference to' when I inline?
	const std::string& getName() const;
	const std::string& getDescription() const;

	virtual ~ClientSpellData() = default;
};

#endif  // _CLIENT_CARD_DATA_CLIENT_HPP
