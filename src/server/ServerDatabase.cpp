#include "server/ServerDatabase.hpp"

#include <cstring>

#define AUTO_QUERY_LENGTH -1

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
const char ServerDatabase::FRIEND_LIST_QUERY[] = "SELECT id,login FROM Friendship INNER JOIN Account ON second == id WHERE first == ?;";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	sqliteThrowExcept(sqlite3_prepare_v2(_database, FRIEND_LIST_QUERY, sizeof(FRIEND_LIST_QUERY),
	                                     &friendListStmt, nullptr));

	/* TEST */
	std::cout << getFriendsList(1)->front().name << std::endl;
}

const FriendsList * ServerDatabase::getFriendsList(const int user)
{
	// TODO: cache - size of result
	sqlite3_reset(friendListStmt);
	sqlite3_bind_int(friendListStmt, 1, user);

	FriendsList * friends = new FriendsList;

	while(sqliteThrowExcept(sqlite3_step(friendListStmt)) == SQLITE_ROW)
	{
		friends->emplace_back(Friend {sqlite3_column_int(friendListStmt, 0), // id
		                              reinterpret_cast<const char *>(sqlite3_column_text(friendListStmt, 1)) // name
		                             });
	}

	return friends;
}

ServerDatabase::~ServerDatabase()
{
	if(sqlite3_finalize(friendListStmt) != SQLITE_OK)
		std::cerr << "ERROR while finalizing statements" << std::endl;
}

// TODO Monster to Creature
