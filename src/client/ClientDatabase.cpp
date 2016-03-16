#include "client/ClientDatabase.hpp"

// WizardPocker
#include "client/ClientCreature.hpp"
#include "client/ClientSpell.hpp"
// std-C++
#include <cassert>

const char ClientDatabase::FILENAME[] = "../resources/client/database.db";
ClientDatabase::ClientDatabase(const std::string& filename) : Database(filename)
{
	prepareStmt(_getCardFullStmt);
}

Card ClientDatabase::getCard(cardId id)
{
	const CommonCardData* data(getCardData(id));
	if (data->isSpell())
	{
		return ClientSpell(*static_cast<const ClientSpellData*>(data));
	}
	else
	{
		return ClientCreature(*static_cast<const ClientCreatureData*>(data));
	}
}

const CommonCardData* ClientDatabase::getCardData(cardId id)
{
	auto found = _cards.find(id);

	if(found == _cards.end())
	{
		sqlite3_reset(_getCardStmt);
		sqliteThrowExcept(sqlite3_bind_int64(_getCardStmt, 1, id));

		assert(sqliteThrowExcept(sqlite3_step(_getCardStmt)) == SQLITE_ROW);

		CommonCardData * cardData;
		ClientSpellData test(1, "Nom", 2, "Description"); // TODO delete

		if(sqlite3_column_type(_getCardStmt, 4) == SQLITE_NULL) // Spell
		{
			cardData = new ClientSpellData(
			    id,
			    reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 0)), // name
			    sqlite3_column_int(_getCardStmt, 1), // cost
			    reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 2)) // description
			);
		}
		else // Creature
		{
			cardData = new ClientCreatureData(
			    id,
			    reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 0)), // name
			    sqlite3_column_int(_getCardStmt, 1), // cost
			    reinterpret_cast<const char *>(sqlite3_column_text(_getCardStmt, 2)), // description
			    sqlite3_column_int(_getCardStmt, 3), // attack
			    sqlite3_column_int(_getCardStmt, 4), // health
			    sqlite3_column_int(_getCardStmt, 5), // shield
			    sqlite3_column_int(_getCardStmt, 6) // shieldType
			);
		}

		return _cards.emplace(std::make_pair<>(
		                          id,
		                          std::unique_ptr<CommonCardData>(cardData)
		                      )).first->second.get();
	}
	else
	{
		return found->second.get();
	}
}

ClientDatabase::~ClientDatabase()
{
	int errcode;

	if((errcode = sqlite3_finalize(_getCardStmt)) != SQLITE_OK)
		std::cerr << "ERROR while finalizing statement "
		          << ": " << sqlite3_errstr(errcode)
		          << std::endl;

	/*for(_cards::const_iterator it = _cards.begin(); it != _cards.end(); ++it)
		delete *it;*/
}
