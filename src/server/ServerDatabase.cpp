#include "server/ServerDatabase.hpp"

#include <cstring>

#define AUTO_QUERY_LENGTH -1

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
const char ServerDatabase::FRIEND_LIST_QUERY[] =
    "SELECT id,login "
    "FROM Friendship "
    "INNER JOIN Account ON second == id "
    "WHERE first == ?1;";
const char ServerDatabase::USER_ID_QUERY[] =
    "SELECT id FROM Account WHERE login == ?1;";
const char ServerDatabase::LOGIN_QUERY[] =
    "SELECT login FROM Account WHERE id == ?1;";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	prepareStmt(FRIEND_LIST_QUERY, &friendListStmt);
	prepareStmt(USER_ID_QUERY, &userIdStmt);
	prepareStmt(LOGIN_QUERY, &loginStmt);

	/* TEST */
	std::cout << getFriendsList(1)->front().name << std::endl;
	std::cout << getUserId("testing") << getUserId("testing2") << std::endl;
	std::cout << getLogin(1) << getLogin(2) << std::endl;
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

int ServerDatabase::getUserId(const std::string login)
{
	sqlite3_reset(userIdStmt);
	sqlite3_bind_text(userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT);

	if(sqliteThrowExcept(sqlite3_step(userIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int(userIdStmt, 0);
}

std::string ServerDatabase::getLogin(const int userId)
{
	sqlite3_reset(loginStmt);
	sqlite3_bind_int(loginStmt, 1, userId);

	if(sqliteThrowExcept(sqlite3_step(loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR userId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(loginStmt, 0));
}

ServerDatabase::~ServerDatabase()
{
	if(sqlite3_finalize(friendListStmt) != SQLITE_OK
	        || sqlite3_finalize(userIdStmt) != SQLITE_OK
	        || sqlite3_finalize(loginStmt) != SQLITE_OK)
		std::cerr << "ERROR while finalizing statements" << std::endl;
}

void ServerDatabase::prepareStmt(const char* const query, sqlite3_stmt ** stmt)
{
	sqliteThrowExcept(sqlite3_prepare_v2(_database, query, std::strlen(query), stmt, nullptr));
}

// TODO Monster to Creature
