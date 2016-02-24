#include "server/ServerDatabase.hpp"

#ifndef NDEBUG
#include <cassert>
#endif

#define AUTO_QUERY_LENGTH -1

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	for(int i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);

#ifndef NDEBUG
// Unit tests still to do
	std::cout << "DEBUG" << std::endl;

	assert(getFriendsList(1).front().name == "testing2");
	assert(getUserId("testing") == 1);
	assert(getUserId("testing2") == 2);
	assert(getLogin(1) == "testing");
	assert(getLogin(2) == "testing2");
#endif
}

int ServerDatabase::getUserId(const std::string login)
{
	sqlite3_reset(_userIdStmt);
	sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT);

	if(sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int(_userIdStmt, 0);
}

std::string ServerDatabase::getLogin(const int userId)
{
	sqlite3_reset(_loginStmt);
	sqlite3_bind_int(_loginStmt, 1, userId);

	if(sqliteThrowExcept(sqlite3_step(_loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR userId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(_loginStmt, 0));
}

FriendsList ServerDatabase::getFriendsList(const int user)
{
	// TODO: cache - size of result
	sqlite3_reset(_friendListStmt);
	sqlite3_bind_int(_friendListStmt, 1, user);

	FriendsList friends;

	while(sqliteThrowExcept(sqlite3_step(_friendListStmt)) == SQLITE_ROW)
	{
		friends.emplace_back(Friend {sqlite3_column_int(_friendListStmt, 0), // id
		                              reinterpret_cast<const char *>(sqlite3_column_text(_friendListStmt, 1)) // name
		                             });
	}

	return friends;
}

ServerDatabase::~ServerDatabase()
{
	// TODO: move it to Database::~Database
	int errcode;
	for(int i = 0; i < _statements.size(); ++i)
		if((errcode = sqlite3_finalize(*_statements[i].statement())) != SQLITE_OK)
			std::cerr << "ERROR while finalizing statement "
			          << i+1 << " of " << _statements.size()
			          << ": " << sqlite3_errstr(errcode)
			          << std::endl;
}

// TODO Monster to Creature
