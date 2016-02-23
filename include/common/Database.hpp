#ifndef _DATABASE_COMMON_HPP
#define _DATABASE_COMMON_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <sqlite3.h>

struct Friend
{
	int64_t id;
	std::string name;
};
typedef std::vector<Friend> FriendsList;

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
	int sqliteThrowExcept(int errcode) const;

	sqlite3 *_database;
};

#endif // _DATABASE_COMMON_HPP
