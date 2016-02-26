#include "server/ServerDatabase.hpp"

#include <cassert>

#define AUTO_QUERY_LENGTH -1

// TODO: this is multi-threaded
// Rien à voir mais j'y pense maintenant: ajouter aux requirements que l'utilisateur doit pouvoir supprimer toutes ses données personnelles

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);
}

int ServerDatabase::getUserId(const std::string login)
{
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	if(sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int(_userIdStmt, 0);
}

std::string ServerDatabase::getLogin(const int userId)
{
	sqlite3_reset(_loginStmt);
	sqliteThrowExcept(sqlite3_bind_int(_loginStmt, 1, userId));

	if(sqliteThrowExcept(sqlite3_step(_loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR userId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(_loginStmt, 0));
}

Ladder ServerDatabase::getLadder()
{
	sqlite3_reset(_ladderStmt);
	sqliteThrowExcept(sqlite3_bind_int(_ladderStmt, 1, ladderSize));

	Ladder ladder;

	for(size_t i = 0; i < ladder.size() && sqliteThrowExcept(sqlite3_step(_ladderStmt)) == SQLITE_ROW; ++i)
	{
		ladder[i].name = reinterpret_cast<const char *>(sqlite3_column_text(_ladderStmt, 0));
		ladder[i].victories = sqlite3_column_int(_ladderStmt, 1);
		ladder[i].defeats = sqlite3_column_int(_ladderStmt, 2);
	}

	return ladder;
}

void ServerDatabase::addFriend(const int userId1, const int userId2)
{
	sqlite3_reset(_addFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addFriendStmt, 1, userId1));
	sqliteThrowExcept(sqlite3_bind_int(_addFriendStmt, 2, userId2));

	sqliteThrowExcept(sqlite3_step(_addFriendStmt));
}

void ServerDatabase::removeFriend(const int userId1, const int userId2)
{
	sqlite3_reset(_removeFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendStmt, 1, userId1 < userId2 ? userId1 : userId2));
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendStmt, 2, userId1 < userId2 ? userId2 : userId1));

	sqliteThrowExcept(sqlite3_step(_removeFriendStmt));
	assert(sqlite3_step(_removeFriendStmt) == SQLITE_DONE);
}

bool ServerDatabase::areFriend(const int userId1, const int userId2)
{
	sqlite3_reset(_areFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_areFriendStmt, 1, userId1));
	sqliteThrowExcept(sqlite3_bind_int(_areFriendStmt, 2, userId2));

	return sqliteThrowExcept(sqlite3_step(_areFriendStmt)) == SQLITE_ROW;
}

void ServerDatabase::addFriendshipRequest(const int from, const int to)
{
	sqlite3_reset(_addFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int(_addFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_addFriendshipRequestStmt)) == SQLITE_DONE);
}

void ServerDatabase::removeFriendshipRequest(const int from, const int to)
{
	sqlite3_reset(_removeFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_removeFriendshipRequestStmt)) == SQLITE_DONE);
}

bool ServerDatabase::isFriendshipRequestSent(const int from, const int to)
{
	sqlite3_reset(_isFriendshipRequestSentStmt);
	sqliteThrowExcept(sqlite3_bind_int(_isFriendshipRequestSentStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int(_isFriendshipRequestSentStmt, 2, to));

	return sqliteThrowExcept(sqlite3_step(_isFriendshipRequestSentStmt)) == SQLITE_ROW;
}

CardsCollection ServerDatabase::getCardsCollection(const int userId)
{
	sqlite3_reset(_cardsCollectionStmt);
	sqliteThrowExcept(sqlite3_bind_int(_cardsCollectionStmt, 1, userId));

	CardsCollection cards;

	while(sqliteThrowExcept(sqlite3_step(_cardsCollectionStmt)) == SQLITE_ROW)
	{
		cards.addCard(sqlite3_column_int(_cardsCollectionStmt, 0));
	}

	return cards;
}

std::vector<Deck> ServerDatabase::getDecks(const int userId)
{
	sqlite3_reset(_decksStmt);
	sqliteThrowExcept(sqlite3_bind_int(_decksStmt, 1, userId));

	std::vector<Deck> decks;

	while(sqliteThrowExcept(sqlite3_step(_decksStmt)) == SQLITE_ROW)
	{
		decks.emplace_back(Deck(reinterpret_cast<const char *>(sqlite3_column_text(_decksStmt, 0))));

		for(size_t i {0}; i < Deck::size; ++i)
			decks.back().changeCard(i, static_cast<ClientCard::ID>(sqlite3_column_int(_decksStmt, i + 1)));
	}

	return decks;
}

void ServerDatabase::createDeck(const int userId, const Deck& deck)
{
	sqlite3_reset(_createDeckStmt);
	sqliteThrowExcept(sqlite3_bind_int(_createDeckStmt, 1, userId));
	sqliteThrowExcept(sqlite3_bind_text(_createDeckStmt, 2, deck.getName().c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));

	for(int card = 0; card < Deck::size; ++card)
	{
		sqliteThrowExcept(sqlite3_bind_int(_createDeckStmt, card + 3, deck.getCard(card)));
	}

	assert(sqliteThrowExcept(sqlite3_step(_createDeckStmt)) == SQLITE_DONE);
}

void ServerDatabase::deleteDeckByName(const int userId, const std::string& deckName)
{
	sqlite3_reset(_deleteDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_int(_deleteDeckByNameStmt, 1, userId));
	sqliteThrowExcept(sqlite3_bind_text(_deleteDeckByNameStmt, 2, deckName.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_deleteDeckByNameStmt)) == SQLITE_DONE);
}

void ServerDatabase::editDeck(const int userId, const Deck& deck)
{
	sqlite3_reset(_editDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_text(_editDeckByNameStmt, 1, deck.getName().c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	for(int card = 0; card < Deck::size; ++card)
	{
		sqliteThrowExcept(sqlite3_bind_int(_editDeckByNameStmt, card + 2, deck.getCard(card)));
	}
	sqliteThrowExcept(sqlite3_bind_int(_editDeckByNameStmt, 22, userId));

	assert(sqliteThrowExcept(sqlite3_step(_editDeckByNameStmt)) == SQLITE_DONE);
}

bool ServerDatabase::areIdentifiersValid(const std::string& login, const std::string& password)
{
	sqlite3_reset(_areIdentifiersValidStmt);
	sqliteThrowExcept(sqlite3_bind_text(_areIdentifiersValidStmt, 1, login.c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_areIdentifiersValidStmt, 2, password.c_str(), sizeof(password.c_str()),
	                                    SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_areIdentifiersValidStmt)) == SQLITE_ROW;
}

bool ServerDatabase::isRegistered(const std::string& login)
{
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_ROW;
}

void ServerDatabase::registerUser(const std::string& login, const std::string& password)
{
	sqlite3_reset(_registerUserStmt);
	sqliteThrowExcept(sqlite3_bind_text(_registerUserStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_registerUserStmt, 2, password.c_str(), sizeof(password.c_str()),
	                                    SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_registerUserStmt)) == SQLITE_DONE);
}

FriendsList ServerDatabase::getAnyFriendsList(const int user, sqlite3_stmt * stmt)
{
	sqlite3_reset(stmt);
	sqliteThrowExcept(sqlite3_bind_int(stmt, 1, user));

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
