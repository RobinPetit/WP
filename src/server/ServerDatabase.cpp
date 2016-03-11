#include "common/Identifiers.hpp"
#include "server/ServerDatabase.hpp"
#include "server/Spell.hpp"
#include "server/Card.hpp"
#include "server/Creature.hpp"

#include <cassert>
#include <cstring>
#include <string>
#include <utility>

#define AUTO_QUERY_LENGTH -1

// TODO: this is multi-threaded

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(std::string filename) : Database(filename), _cards()
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);

	createSpellCards();
	createCreatureCards();
}

Card* ServerDatabase::getCard(cardId card)
{
	if(_cards.count(card) == 0)
		throw std::runtime_error("The requested card (" + std::to_string(card) + ") does not exist.");

	return _cards.at(card).get();
}

userId ServerDatabase::getUserId(const std::string login)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	if(sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int64(_userIdStmt, 0);
}

std::string ServerDatabase::getLogin(userId id)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_loginStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_loginStmt, 1, id));

	if(sqliteThrowExcept(sqlite3_step(_loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR userId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(_loginStmt, 0));
}

std::vector<Deck> ServerDatabase::getDecks(userId id)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_decksStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_decksStmt, 1, id));

	std::vector<Deck> decks;

	while(sqliteThrowExcept(sqlite3_step(_decksStmt)) == SQLITE_ROW)
	{
		decks.emplace_back(Deck(reinterpret_cast<const char *>(sqlite3_column_text(_decksStmt, 0))));

		for(int i {0}; i < static_cast<int>(Deck::size); ++i)
			decks.back().changeCard(i, sqlite3_column_int64(_decksStmt, i + 1));
	}

	return decks;
}

CardsCollection ServerDatabase::getCardsCollection(userId id)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_cardsCollectionStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_cardsCollectionStmt, 1, id));

	CardsCollection cards;

	while(sqliteThrowExcept(sqlite3_step(_cardsCollectionStmt)) == SQLITE_ROW)
	{
		cards.addCard(sqlite3_column_int64(_cardsCollectionStmt, 0));
	}

	return cards;
}

void ServerDatabase::addCard(userId id, cardId card)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_newCardStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_newCardStmt, 1, card));
	sqliteThrowExcept(sqlite3_bind_int64(_newCardStmt, 2, id));

	assert(sqliteThrowExcept(sqlite3_step(_newCardStmt)) == SQLITE_DONE);
}

Ladder ServerDatabase::getLadder()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
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
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_addFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendStmt, 1, userId1));
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendStmt, 2, userId2));

	sqliteThrowExcept(sqlite3_step(_addFriendStmt));
}

void ServerDatabase::removeFriend(userId userId1, userId userId2)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_removeFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendStmt, 1, userId1 < userId2 ? userId1 : userId2));
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendStmt, 2, userId1 < userId2 ? userId2 : userId1));

	sqliteThrowExcept(sqlite3_step(_removeFriendStmt));
	assert(sqlite3_step(_removeFriendStmt) == SQLITE_DONE);
}

bool ServerDatabase::areFriend(userId userId1, userId userId2)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_areFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_areFriendStmt, 1, userId1));
	sqliteThrowExcept(sqlite3_bind_int64(_areFriendStmt, 2, userId2));

	return sqliteThrowExcept(sqlite3_step(_areFriendStmt)) == SQLITE_ROW;
}

void ServerDatabase::addFriendshipRequest(userId from, userId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_addFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_addFriendshipRequestStmt)) == SQLITE_DONE);
}

void ServerDatabase::removeFriendshipRequest(userId from, userId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_removeFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_removeFriendshipRequestStmt)) == SQLITE_DONE);
}

bool ServerDatabase::isFriendshipRequestSent(userId from, userId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_isFriendshipRequestSentStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_isFriendshipRequestSentStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_isFriendshipRequestSentStmt, 2, to));

	return sqliteThrowExcept(sqlite3_step(_isFriendshipRequestSentStmt)) == SQLITE_ROW;
}

Deck ServerDatabase::getDeckByName(userId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock {_dbAccess};

	// TODO this is certainly not the best way to get an unique deck from the DB
	for(auto & deck : getDecks(id))
		if(deck.getName() == deckName)
			return deck;

	return Deck();
}

void ServerDatabase::createDeck(userId id, const Deck& deck)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_createDeckStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_createDeckStmt, 1, id));
	sqliteThrowExcept(sqlite3_bind_text(_createDeckStmt, 2, deck.getName().c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));

	for(auto card = 0U; card < Deck::size; ++card)
	{
		sqliteThrowExcept(sqlite3_bind_int64(_createDeckStmt, card + 3, deck.getCard(card)));
	}

	assert(sqliteThrowExcept(sqlite3_step(_createDeckStmt)) == SQLITE_DONE);
}

void ServerDatabase::deleteDeckByName(userId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_deleteDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_deleteDeckByNameStmt, 1, id));
	sqliteThrowExcept(sqlite3_bind_text(_deleteDeckByNameStmt, 2, deckName.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_deleteDeckByNameStmt)) == SQLITE_DONE);
}

void ServerDatabase::editDeck(userId id, const Deck& deck)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_editDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_text(_editDeckByNameStmt, 1, deck.getName().c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	for(auto card = 0U; card < Deck::size; ++card)
		sqliteThrowExcept(sqlite3_bind_int64(_editDeckByNameStmt, card + 2, deck.getCard(card)));

	sqliteThrowExcept(sqlite3_bind_int64(_editDeckByNameStmt, 22, id));

	assert(sqliteThrowExcept(sqlite3_step(_editDeckByNameStmt)) == SQLITE_DONE);
}

bool ServerDatabase::areIdentifiersValid(const std::string& login, const std::string& password)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_areIdentifiersValidStmt);
	sqliteThrowExcept(sqlite3_bind_text(_areIdentifiersValidStmt, 1, login.c_str(), AUTO_QUERY_LENGTH,
	                                    SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_areIdentifiersValidStmt, 2, password.c_str(),
	                                    static_cast<int>(std::strlen(password.c_str())), SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_areIdentifiersValidStmt)) == SQLITE_ROW;
}

bool ServerDatabase::isRegistered(const std::string& login)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_userIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_userIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_userIdStmt)) == SQLITE_ROW;
}

void ServerDatabase::registerUser(const std::string& login, const std::string& password)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_registerUserStmt);
	sqliteThrowExcept(sqlite3_bind_text(_registerUserStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));
	sqliteThrowExcept(sqlite3_bind_blob(_registerUserStmt, 2, password.c_str(),
	                                    static_cast<int>(std::strlen(password.c_str())), SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_registerUserStmt)) == SQLITE_DONE);
}

FriendsList ServerDatabase::getAnyFriendsList(userId user, sqlite3_stmt * stmt)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(stmt);
	sqliteThrowExcept(sqlite3_bind_int64(stmt, 1, user));

	FriendsList friends;

	while(sqliteThrowExcept(sqlite3_step(stmt)) == SQLITE_ROW)
	{
		friends.emplace_back(Friend {sqlite3_column_int64(stmt, 0), // id
		                             reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)) // name
		                            });
	}

	return friends;
}

void ServerDatabase::createSpellCards()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_getSpellCardsStmt);

	while(sqliteThrowExcept(sqlite3_step(_getSpellCardsStmt)) == SQLITE_ROW)
	{
		cardId id(sqlite3_column_int64(_getSpellCardsStmt, 0));

		_cards.emplace(
		    std::make_pair<>(
		        id,
		        std::unique_ptr<Card>(new Spell(
		                                  id, sqlite3_column_int(_getSpellCardsStmt, 1), // cost
		                                  std::vector<EffectParamsCollection>(createCardEffects(id)) // effects
		                              ))
		    )
		);
	}
}

void ServerDatabase::createCreatureCards()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_getCreatureCardsStmt);

	while(sqliteThrowExcept(sqlite3_step(_getCreatureCardsStmt)) == SQLITE_ROW)
	{
		cardId id(sqlite3_column_int64(_getCreatureCardsStmt, 0));

		_cards.emplace(
		    std::make_pair<>(
		        id,
		        std::unique_ptr<Card>(new Creature(
		                                  id,
		                                  sqlite3_column_int(_getCreatureCardsStmt, 1), // cost
		                                  sqlite3_column_int(_getCreatureCardsStmt, 2), // attack
		                                  sqlite3_column_int(_getCreatureCardsStmt, 3), // health
		                                  sqlite3_column_int(_getCreatureCardsStmt, 4), // shield
		                                  sqlite3_column_int(_getCreatureCardsStmt, 5), // shieldType
		                                  std::vector<EffectParamsCollection>(createCardEffects(id)) // effects
		                              ))
		    )
		);
	}
}

std::vector<EffectParamsCollection> ServerDatabase::createCardEffects(cardId id)
{
	sqlite3_reset(_getCardEffectsStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_getCardEffectsStmt, 1, id));

	std::vector<EffectParamsCollection> effects;

	while(sqliteThrowExcept(sqlite3_step(_getCardEffectsStmt)) == SQLITE_ROW)
	{
		effects.push_back(EffectParamsCollection
		{
			sqlite3_column_int(_getCardEffectsStmt, 0),
			sqlite3_column_int(_getCardEffectsStmt, 1),
			sqlite3_column_int(_getCardEffectsStmt, 2),
			sqlite3_column_int(_getCardEffectsStmt, 3),
			sqlite3_column_int(_getCardEffectsStmt, 4),
			sqlite3_column_int(_getCardEffectsStmt, 5),
			sqlite3_column_int(_getCardEffectsStmt, 6),
		});
	}

	return effects;
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
