#include "server/ServerDatabase.hpp"

#ifndef NDEBUG
#include <cassert>
#endif

#define AUTO_QUERY_LENGTH -1

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);
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

std::vector<Deck> ServerDatabase::getDecks(const int userId)
{
	sqlite3_reset(_decksStmt);
	sqlite3_bind_int(_decksStmt, 1, userId);

	std::vector<Deck> decks;

	while(sqliteThrowExcept(sqlite3_step(_decksStmt)) == SQLITE_ROW)
	{
		decks.emplace_back(Deck(reinterpret_cast<const char *>(sqlite3_column_text(_decksStmt, 0))));

		for(size_t i {0}; i < Deck::size; ++i)
			decks.back().changeCard(i, static_cast<Card::ID>(sqlite3_column_int(_decksStmt, i + 1)));
	}

	return decks;
}

CardsCollection ServerDatabase::getCardsCollection(const int userId)
{
	sqlite3_reset(_cardsCollectionStmt);
	sqlite3_bind_int(_cardsCollectionStmt, 1, userId);

	CardsCollection cards;

	while(sqliteThrowExcept(sqlite3_step(_cardsCollectionStmt)) == SQLITE_ROW)
	{
		cards.addCard(sqlite3_column_int(_cardsCollectionStmt, 0));
	}

	return cards;
}

Ladder ServerDatabase::getLadder()
{
	sqlite3_reset(_ladderStmt);
	sqlite3_bind_int(_ladderStmt, 1, ladderSize);

	Ladder ladder;

	for(size_t i = 0; i < ladder.size() && sqliteThrowExcept(sqlite3_step(_ladderStmt)) == SQLITE_ROW; ++i)
	{
		ladder[i].name = reinterpret_cast<const char *>(sqlite3_column_text(_ladderStmt, 0));
		ladder[i].victories = sqlite3_column_int(_ladderStmt, 1);
		ladder[i].defeats = sqlite3_column_int(_ladderStmt, 2);
	}

	return ladder;
}

void ServerDatabase::addFriend(int userId, const int newFriendId)
{
	sqlite3_reset(_addFriendStmt);
	sqlite3_bind_int(_addFriendStmt, 1, userId);
	sqlite3_bind_int(_addFriendStmt, 2, newFriendId);

	sqlite3_step(_addFriendStmt);
}

FriendsList ServerDatabase::getAnyFriendsList(const int user, sqlite3_stmt * stmt)
{
	sqlite3_reset(stmt);
	sqlite3_bind_int(stmt, 1, user);

	FriendsList friends;

	while(sqliteThrowExcept(sqlite3_step(stmt)) == SQLITE_ROW)
	{
		friends.emplace_back(Friend {sqlite3_column_int(stmt, 0), // id
		                             reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)) // name
		                            });
	}

	return friends;
}

ServerDatabase::~ServerDatabase()
{
	// TODO: move it to Database::~Database
	int errcode;

	for(size_t i = 0; i < _statements.size(); ++i)
		if((errcode = sqlite3_finalize(*_statements[i].statement())) != SQLITE_OK)
			std::cerr << "ERROR while finalizing statement "
			          << i + 1 << " of " << _statements.size()
			          << ": " << sqlite3_errstr(errcode)
			          << std::endl;
}

// TODO Monster to Creature
