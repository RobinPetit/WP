#ifndef _DATABASE_COMMON_HPP
#define _DATABASE_COMMON_HPP

// std-C++ headers
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <utility> // std::pair
#include <mutex>

// SQLite headers
#include <sqlite3.h>
// WizardPoker headers
#include "common/Deck.hpp"
#include "common/CardsCollection.hpp"
#include "common/Identifiers.hpp"

struct LadderEntry
{
	std::string name;
	unsigned victories;
	unsigned defeats;
};

constexpr int ladderSize = 20;
using Ladder =  std::array<LadderEntry, ladderSize>;

struct Statement : private std::pair<sqlite3_stmt **, const char *> // I dont care that it is a std::pair (this is just for implementation) and I prefer apply maximum restrictions rule
{
	Statement(first_type statement, second_type query)
		: std::pair<first_type, second_type>(statement, query)
	{
	}

	sqlite3_stmt ** statement()
	{
		return first;
	}

	const char * query() const
	{
		return second;
	}
};

template <std::size_t N>
using StatementsList = std::array<Statement, N>;

/// Interface to the database
class Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file
	explicit Database(std::string filename);

	/// Destructor
	virtual ~Database();

protected:
	/// To get valid sqlite3_stmt
	void prepareStmt(Statement&);

	/// Throw exception if errcode is actually an error code
	int sqliteThrowExcept(int errcode) const;

	sqlite3 *_database;

	/// Avoids race conditions on DB
	std::mutex _dbAccess;
};

struct Friend
{
	userId id;
	std::string name;
};
typedef std::vector<Friend> FriendsList;

#endif // _DATABASE_COMMON_HPP
