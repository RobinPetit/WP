#ifndef _CLIENT_DATABASE_CLIENT_HPP
#define _CLIENT_DATABASE_CLIENT_HPP

#include <map>
#include <memory>

#include "common/Database.hpp"
#include "client/ClientCard.hpp"

/// Interface to the client database.
class ClientDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ClientDatabase(const std::string& filename = FILENAME);

	const ClientCard* getCard(cardId id);

	virtual ~ClientDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];

	std::map<const cardId, const std::unique_ptr<const ClientCard> > _cards;

	sqlite3_stmt * _getCardStmt;
	Statement _getCardFullStmt
	{
		&_getCardStmt,
		"SELECT name, cost, description,"
		"		attack, health, shield, shieldType "
		"	FROM FullCard "
		"	WHERE id == ?1;"
	};
};

#endif // _CLIENT_DATABASE_CLIENT_HPP
