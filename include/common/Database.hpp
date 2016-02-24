#ifndef _DATABASE_COMMON_HPP
#define _DATABASE_COMMON_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <utility> // std::pair

#include <sqlite3.h>

#include <common/Deck.hpp>
#include <common/CardsCollection.hpp>

struct Friend
{
	int64_t id;
	std::string name;
};
typedef std::vector<Friend> FriendsList;

struct Statement : private std::pair<sqlite3_stmt **, const char *> // I dont care that it is a std::pair (this is just for implementation) and I prefer apply maximum restrictions rule
{
	Statement(first_type statement, second_type query)
		: std::pair<first_type, second_type>(statement, query) {}
	sqlite3_stmt ** statement()
	{
		return first;
	}
	const char * const query() const
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
};

#endif // _DATABASE_COMMON_HPP
