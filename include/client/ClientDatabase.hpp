#ifndef _CLIENT_DATABASE_CLIENT_HPP
#define _CLIENT_DATABASE_CLIENT_HPP

#include <map>
#include <memory>
#include <vector>

#include "common/Database.hpp"
#include "common/Card.hpp"
#include "client/ClientCardData.hpp"
#include "client/AchievementData.hpp"

/// Interface to the client database.
class ClientDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ClientDatabase(const std::string& filename = FILENAME);

	Card getCard(cardId id);
	const CommonCardData* getCardData(cardId id);
	std::vector<cardId> getFirstCardIds(unsigned count);
	cardId getGreatestCardId();

	cardId countCards();
	cardId countCreatures();
	cardId countSpells();

	AchievementData getAchievementData(AchievementId id);

	virtual ~ClientDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];

	std::map<const cardId, const std::unique_ptr<const CommonCardData> > _cards;
	int _cardCount;
	int _creatureCount;
	int _spellCount;

	sqlite3_stmt * _getCardStmt;
	sqlite3_stmt * _countCardsStmt;
	sqlite3_stmt * _countCreaturesStmt;
	sqlite3_stmt * _countSpellsStmt;
	sqlite3_stmt * _getFirstCardIdsStmt;
	sqlite3_stmt * _getGreatestCardIdStmt;
	sqlite3_stmt * _getAchievementDataStmt;
	//TODO (server too) separate startup statement from long-life statement to save (a few) memory
	StatementsList<7> _statements
	{
		{
			Statement {
				&_getCardStmt,
				"SELECT name, cost, description,"
				"		attack, health, shield, shieldType "
				"	FROM FullCard "
				"	WHERE id == ?1;"
			},
			Statement {
				&_countCardsStmt,
				"SELECT count() FROM FullCard;"
			},
			Statement {
				&_countCreaturesStmt,
				"SELECT count() FROM CreatureCard;"
			},
			Statement { // 3
				&_countSpellsStmt,
				"SELECT count() FROM SpellCard;"
			},
			Statement {
				&_getFirstCardIdsStmt,
				"SELECT id "
				"	FROM FullCard "
				"	ORDER BY id "
				"	LIMIT ?1;"
			},
			Statement {
				&_getGreatestCardIdStmt,
				"SELECT id "
				"	FROM FullCard "
				"	ORDER BY id DESC "
				"	LIMIT 1;"
			},
			Statement {
				&_getAchievementDataStmt,
				"SELECT name, description, progressRequired "
				"	FROM Achievement "
				"	WHERE id == ?1;"
			}
		}
	};
};

#endif // _CLIENT_DATABASE_CLIENT_HPP
