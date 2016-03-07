#include "server/ServerDatabase.hpp"
#include "common/Identifiers.hpp"

#include <cassert>
#include <cstring>

#define AUTO_QUERY_LENGTH -1

// TODO: this is multi-threaded

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename)
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);
}

userId ServerDatabase::getUserId(const std::string login)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	if(sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int(_userIdStmt, 0);
}

std::string ServerDatabase::getLogin(userId id)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_loginStmt);
	sqliteThrowExcept(sqlite3_bind_int(_loginStmt, 1, static_cast<int>(id)));

	if(sqliteThrowExcept(sqlite3_step(_loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR userId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(_loginStmt, 0));
}

std::vector<Deck> ServerDatabase::getDecks(userId id)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_decksStmt);
	sqliteThrowExcept(sqlite3_bind_int(_decksStmt, 1, static_cast<int>(id)));

	std::vector<Deck> decks;

	while(sqliteThrowExcept(sqlite3_step(_decksStmt)) == SQLITE_ROW)
	{
		decks.emplace_back(Deck(reinterpret_cast<const char *>(sqlite3_column_text(_decksStmt, 0))));

		for(size_t i {0}; i < Deck::size; ++i)
			decks.back().changeCard(i, static_cast<cardId>(sqlite3_column_int(_decksStmt, i + 1)));
	}

	return decks;
}

CardsCollection ServerDatabase::getCardsCollection(userId id)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_cardsCollectionStmt);
	sqliteThrowExcept(sqlite3_bind_int(_cardsCollectionStmt, 1, static_cast<int>(id)));

	CardsCollection cards;

	while(sqliteThrowExcept(sqlite3_step(_cardsCollectionStmt)) == SQLITE_ROW)
	{
		cards.addCard(sqlite3_column_int(_cardsCollectionStmt, 0));
	}

	return cards;
}

void ServerDatabase::addCard(userId id, cardId card)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_newCardStmt);
	sqliteThrowExcept(sqlite3_bind_int(_newCardStmt, 1, static_cast<int>(card)));
	sqliteThrowExcept(sqlite3_bind_int(_newCardStmt, 2, static_cast<int>(id)));

	assert(sqliteThrowExcept(sqlite3_step(_newCardStmt)) == SQLITE_DONE);
}

Ladder ServerDatabase::getLadder()
{
	std::unique_lock<std::mutex> lock{_dbAccess};
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

void ServerDatabase::addFriend(userId userId1, userId userId2)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_addFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addFriendStmt, 1, static_cast<int>(userId1)));
	sqliteThrowExcept(sqlite3_bind_int(_addFriendStmt, 2, static_cast<int>(userId2)));

	sqliteThrowExcept(sqlite3_step(_addFriendStmt));
}

void ServerDatabase::removeFriend(userId userId1, userId userId2)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_removeFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendStmt, 1, static_cast<int>(userId1 < userId2 ? userId1 : userId2)));
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendStmt, 2, static_cast<int>(userId1 < userId2 ? userId2 : userId1)));

	sqliteThrowExcept(sqlite3_step(_removeFriendStmt));
	assert(sqlite3_step(_removeFriendStmt) == SQLITE_DONE);
}

bool ServerDatabase::areFriend(userId userId1, userId userId2)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_areFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int(_areFriendStmt, 1, static_cast<int>(userId1)));
	sqliteThrowExcept(sqlite3_bind_int(_areFriendStmt, 2, static_cast<int>(userId2)));

	return sqliteThrowExcept(sqlite3_step(_areFriendStmt)) == SQLITE_ROW;
}

void ServerDatabase::addFriendshipRequest(userId from, userId to)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_addFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addFriendshipRequestStmt, 1, static_cast<int>(from)));
	sqliteThrowExcept(sqlite3_bind_int(_addFriendshipRequestStmt, 2, static_cast<int>(to)));

	assert(sqliteThrowExcept(sqlite3_step(_addFriendshipRequestStmt)) == SQLITE_DONE);
}

void ServerDatabase::removeFriendshipRequest(userId from, userId to)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_removeFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendshipRequestStmt, 1, static_cast<int>(from)));
	sqliteThrowExcept(sqlite3_bind_int(_removeFriendshipRequestStmt, 2, static_cast<int>(to)));

	assert(sqliteThrowExcept(sqlite3_step(_removeFriendshipRequestStmt)) == SQLITE_DONE);
}

bool ServerDatabase::isFriendshipRequestSent(userId from, userId to)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_isFriendshipRequestSentStmt);
	sqliteThrowExcept(sqlite3_bind_int(_isFriendshipRequestSentStmt, 1, static_cast<int>(from)));
	sqliteThrowExcept(sqlite3_bind_int(_isFriendshipRequestSentStmt, 2, static_cast<int>(to)));

	return sqliteThrowExcept(sqlite3_step(_isFriendshipRequestSentStmt)) == SQLITE_ROW;
}

Deck ServerDatabase::getDeckByName(userId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	// TODO this is certainly not the best way to get an unique deck from the DB
	for(auto& deck : getDecks(id))
		if(deck.getName() == deckName)
			return deck;
	return Deck();
}

void ServerDatabase::createDeck(userId id, const Deck& deck)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_createDeckStmt);
	sqliteThrowExcept(sqlite3_bind_int(_createDeckStmt, 1, static_cast<int>(id)));
	sqliteThrowExcept(sqlite3_bind_text(_createDeckStmt, 2, deck.getName().c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));

	for(auto card = 0U; card < Deck::size; ++card)
	{
		sqliteThrowExcept(sqlite3_bind_int(_createDeckStmt, card + 3, static_cast<int>(deck.getCard(card))));
	}

	assert(sqliteThrowExcept(sqlite3_step(_createDeckStmt)) == SQLITE_DONE);
}

void ServerDatabase::deleteDeckByName(userId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_deleteDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_int(_deleteDeckByNameStmt, 1, static_cast<int>(id)));
	sqliteThrowExcept(sqlite3_bind_text(_deleteDeckByNameStmt, 2, deckName.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_deleteDeckByNameStmt)) == SQLITE_DONE);
}

void ServerDatabase::editDeck(userId id, const Deck& deck)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_editDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_text(_editDeckByNameStmt, 1, deck.getName().c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	for(auto card = 0U; card < Deck::size; ++card)
		sqliteThrowExcept(sqlite3_bind_int(_editDeckByNameStmt, card + 2, static_cast<int>(deck.getCard(card))));
	sqliteThrowExcept(sqlite3_bind_int(_editDeckByNameStmt, 22, static_cast<int>(id)));

	assert(sqliteThrowExcept(sqlite3_step(_editDeckByNameStmt)) == SQLITE_DONE);
}

bool ServerDatabase::areIdentifiersValid(const std::string& login, const std::string& password)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_areIdentifiersValidStmt);
	sqliteThrowExcept(sqlite3_bind_text(_areIdentifiersValidStmt, 1, login.c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_areIdentifiersValidStmt, 2, password.c_str(), std::strlen(password.c_str()),
	                                    SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_areIdentifiersValidStmt)) == SQLITE_ROW;
}

bool ServerDatabase::isRegistered(const std::string& login)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_ROW;
}

void ServerDatabase::registerUser(const std::string& login, const std::string& password)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(_registerUserStmt);
	sqliteThrowExcept(sqlite3_bind_text(_registerUserStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_registerUserStmt, 2, password.c_str(), std::strlen(password.c_str()),
	                                    SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_registerUserStmt)) == SQLITE_DONE);
}

FriendsList ServerDatabase::getAnyFriendsList(userId user, sqlite3_stmt * stmt)
{
	std::unique_lock<std::mutex> lock{_dbAccess};
	sqlite3_reset(stmt);
	sqliteThrowExcept(sqlite3_bind_int(stmt, 1, static_cast<int>(user)));

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
