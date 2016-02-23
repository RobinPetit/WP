#ifndef _SERVER_DATABASE_SERVER_HPP
#define _SERVER_DATABASE_SERVER_HPP

#include "common/Database.hpp"

/// Interface to the server database.
class ServerDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ServerDatabase(std::string filename = FILENAME);

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];
};

#endif //_DATABASE_SERVER_HPP
