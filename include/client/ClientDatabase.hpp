#ifndef _CLIENT_DATABASE_CLIENT_HPP
#define _CLIENT_DATABASE_CLIENT_HPP

#include "common/Database.hpp"

/// Interface to the client database.
class ClientDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ClientDatabase(const std::string filename = FILENAME);

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];
};

#endif // _CLIENT_DATABASE_CLIENT_HPP
