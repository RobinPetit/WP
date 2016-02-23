#ifndef _DATABASE_COMMON_HPP
#define _DATABASE_COMMON_HPP

#include <iostream>
#include <stdexcept>
#include <string>

#include <sqlite3.h>

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
