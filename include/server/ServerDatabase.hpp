#ifndef _SERVER_DATABASE_SERVER_HPP
#define _SERVER_DATABASE_SERVER_HPP

#include "common/Database.hpp"

// for the moment login and name are used for the same string

/// Interface to the server database.
class ServerDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ServerDatabase(const std::string filename = FILENAME);

	const FriendsList * getFriendsList(const int userId); // If the const of return value bothers you I can remove it, it is for a potential future enhancement

	virtual ~ServerDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];
	
	static const char FRIEND_LIST_QUERY[];
	sqlite3_stmt * friendListStmt;
};

#endif //_DATABASE_SERVER_HPP
