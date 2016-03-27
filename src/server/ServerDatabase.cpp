#include "server/ServerDatabase.hpp"

// WizardPocker
#include "common/Identifiers.hpp"
#include "common/Card.hpp"
#include "server/Spell.hpp"
#include "server/Creature.hpp"
// std-C++
#include <cassert>
#include <cstring>
#include <string>
#include <utility>

#define AUTO_QUERY_LENGTH -1

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ServerDatabase::ServerDatabase(const std::string& filename) :
	Database(filename),
	_cardData(),
	_achievementManager(*this)
{
	for(size_t i = 0; i < _statements.size(); ++i)
		prepareStmt(_statements[i]);

	// The server will need all cards. So we create all at startup and keep its in a map.
	createSpellData();
	createCreatureData();
}

Card* ServerDatabase::getCard(CardId card, Player& owner)
{
	if(_cardData.count(card) == 0)
		throw std::runtime_error("The requested card (" + std::to_string(card) + ") does not exist.");

	// Do not use ?: operator (http://en.cppreference.com/w/cpp/language/operator_other#Conditional_operator)
	if(_cardData.at(card)->isCreature())
		return new Creature(*static_cast<const ServerCreatureData *>(_cardData.at(card).get()), owner);
	else
		return new Spell(*static_cast<const ServerSpellData *>(_cardData.at(card).get()));
}

CardId ServerDatabase::countCards()
{
	sqlite3_reset(_countCardsStmt);
	assert(sqliteThrowExcept(sqlite3_step(_countCardsStmt)) == SQLITE_ROW);
	return sqlite3_column_int(_countCardsStmt, 0);
}

CardId ServerDatabase::getRandomCardId()
{
	sqlite3_reset(_getRandomCardIdStmt);
	assert(sqliteThrowExcept(sqlite3_step(_getRandomCardIdStmt)) == SQLITE_ROW);
	return sqlite3_column_int(_getRandomCardIdStmt, 0);
}

UserId ServerDatabase::getUserId(const std::string& login)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_UserIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_UserIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	if(sqliteThrowExcept(sqlite3_step(_UserIdStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR login not found");

	return sqlite3_column_int64(_UserIdStmt, 0);
}

std::string ServerDatabase::getLogin(UserId id)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_loginStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_loginStmt, 1, id));

	if(sqliteThrowExcept(sqlite3_step(_loginStmt)) == SQLITE_DONE)
		throw std::runtime_error("ERROR UserId not found");

	return reinterpret_cast<const char *>(sqlite3_column_text(_loginStmt, 0));
}

std::vector<Deck> ServerDatabase::getDecks(UserId id)
{
	// std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_decksStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_decksStmt, 1, id));

	std::vector<Deck> decks;

	while(sqliteThrowExcept(sqlite3_step(_decksStmt)) == SQLITE_ROW)
	{
		decks.emplace_back(Deck(reinterpret_cast<const char *>(sqlite3_column_text(_decksStmt, 0))));

		for(int i {0}; i < static_cast<int>(Deck::size); ++i)
			decks.back().changeCard(i, sqlite3_column_int64(_decksStmt, i + 1), false);
	}

	return decks;
}

CardsCollection ServerDatabase::getCardsCollection(UserId id)
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

void ServerDatabase::addCard(UserId id, CardId card)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_newCardStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_newCardStmt, 1, card));
	sqliteThrowExcept(sqlite3_bind_int64(_newCardStmt, 2, id));

	assert(sqliteThrowExcept(sqlite3_step(_newCardStmt)) == SQLITE_DONE);
}

void ServerDatabase::addFriend(UserId UserId1, UserId UserId2)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_addFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendStmt, 1, UserId1));
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendStmt, 2, UserId2));

	sqliteThrowExcept(sqlite3_step(_addFriendStmt));
}

void ServerDatabase::removeFriend(UserId UserId1, UserId UserId2)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_removeFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendStmt, 1, UserId1 < UserId2 ? UserId1 : UserId2));
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendStmt, 2, UserId1 < UserId2 ? UserId2 : UserId1));

	sqliteThrowExcept(sqlite3_step(_removeFriendStmt));
	assert(sqlite3_step(_removeFriendStmt) == SQLITE_DONE);
}

bool ServerDatabase::areFriend(UserId UserId1, UserId UserId2)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_areFriendStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_areFriendStmt, 1, UserId1));
	sqliteThrowExcept(sqlite3_bind_int64(_areFriendStmt, 2, UserId2));

	return sqliteThrowExcept(sqlite3_step(_areFriendStmt)) == SQLITE_ROW;
}

void ServerDatabase::addFriendshipRequest(UserId from, UserId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_addFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_addFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_addFriendshipRequestStmt)) == SQLITE_DONE);
}

void ServerDatabase::removeFriendshipRequest(UserId from, UserId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_removeFriendshipRequestStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendshipRequestStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_removeFriendshipRequestStmt, 2, to));

	assert(sqliteThrowExcept(sqlite3_step(_removeFriendshipRequestStmt)) == SQLITE_DONE);
}

bool ServerDatabase::isFriendshipRequestSent(UserId from, UserId to)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_isFriendshipRequestSentStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_isFriendshipRequestSentStmt, 1, from));
	sqliteThrowExcept(sqlite3_bind_int64(_isFriendshipRequestSentStmt, 2, to));

	return sqliteThrowExcept(sqlite3_step(_isFriendshipRequestSentStmt)) == SQLITE_ROW;
}

Deck ServerDatabase::getDeckByName(UserId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock {_dbAccess};

	// TODO this is certainly not the best way to get an unique deck from the DB
	for(auto & deck : getDecks(id))
		if(deck.getName() == deckName)
			return deck;

	// Throw an exception here?
	return Deck();
}

void ServerDatabase::createDeck(UserId id, const Deck& deck)
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

std::vector<CardId> ServerDatabase::getFirstCardIds(unsigned count)
{
	sqlite3_reset(_getFirstCardIdsStmt);
	sqliteThrowExcept(sqlite3_bind_int(_getFirstCardIdsStmt, 1, static_cast<int>(count)));

	std::vector<CardId> CardIds;

	while(sqliteThrowExcept(sqlite3_step(_getFirstCardIdsStmt)) == SQLITE_ROW)
	{
		CardIds.emplace_back(sqlite3_column_int64(_getFirstCardIdsStmt, 0));
	}

	return CardIds;
}

void ServerDatabase::deleteDeckByName(UserId id, const std::string& deckName)
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_deleteDeckByNameStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_deleteDeckByNameStmt, 1, id));
	sqliteThrowExcept(sqlite3_bind_text(_deleteDeckByNameStmt, 2, deckName.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	assert(sqliteThrowExcept(sqlite3_step(_deleteDeckByNameStmt)) == SQLITE_DONE);
}

void ServerDatabase::editDeck(UserId id, const Deck& deck)
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
	sqlite3_reset(_UserIdStmt);
	sqliteThrowExcept(sqlite3_bind_text(_UserIdStmt, 1, login.c_str(), AUTO_QUERY_LENGTH, SQLITE_TRANSIENT));

	return sqliteThrowExcept(sqlite3_step(_UserIdStmt)) == SQLITE_ROW;
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

FriendsList ServerDatabase::getAnyFriendsList(UserId user, sqlite3_stmt * stmt)
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

void ServerDatabase::createSpellData()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_getSpellCardsStmt);

	while(sqliteThrowExcept(sqlite3_step(_getSpellCardsStmt)) == SQLITE_ROW)
	{
		CardId id(sqlite3_column_int64(_getSpellCardsStmt, 0));

		_cardData.emplace(
		    std::make_pair<>(
		        id,
		        std::unique_ptr<CommonCardData>(
		            new ServerSpellData(
		                id,
		                sqlite3_column_int(_getSpellCardsStmt, 1), // cost
		                std::vector<EffectParamsCollection>(createCardEffects(id)) // effects
		            )
		        )
		    )
		);
	}
}

void ServerDatabase::createCreatureData()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_getCreatureCardsStmt);

	while(sqliteThrowExcept(sqlite3_step(_getCreatureCardsStmt)) == SQLITE_ROW)
	{
		CardId id(sqlite3_column_int64(_getCreatureCardsStmt, 0));

		_cardData.emplace(
		    std::make_pair<>(
		        id,
		        std::unique_ptr<CommonCardData>(
		            new ServerCreatureData(
		                id,
		                sqlite3_column_int(_getCreatureCardsStmt, 1),  // cost
		                std::vector<EffectParamsCollection>(createCardEffects(id)),  // effects
		                sqlite3_column_int(_getCreatureCardsStmt, 2),  // attack
		                sqlite3_column_int(_getCreatureCardsStmt, 3),  // health
		                sqlite3_column_int(_getCreatureCardsStmt, 4),  // shield
		                sqlite3_column_int(_getCreatureCardsStmt, 5)  // shieldType
		            )
		        )
		    )
		);
	}
}

std::vector<EffectParamsCollection> ServerDatabase::createCardEffects(CardId id)
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

unsigned ServerDatabase::countAccounts()
{
	sqlite3_reset(_countAccountsStmt);
	assert(sqliteThrowExcept(sqlite3_step(_countAccountsStmt)) == SQLITE_ROW);
	return sqlite3_column_int(_countAccountsStmt, 0);
}

// Achievements
AchievementList ServerDatabase::newAchievements(const PostGameData& postGame, UserId user)
{
	// (re-)unlock a card (it is a special achievement)
	if(postGame.playerWon)
		addCard(user, getRandomCardId());

	// update LastDayPlayed information (this should be done elsewhere)
	sqlite3_reset(_updateLastDayPlayedStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_updateLastDayPlayedStmt, 1, user));

	assert(sqliteThrowExcept(sqlite3_step(_updateLastDayPlayedStmt)) == SQLITE_DONE);

	// unlock other achievements (unlock a card is a special achievement)
	return _achievementManager.newAchievements(postGame, user);
}

AchievementList ServerDatabase::getAchievements(UserId user)
{
	return _achievementManager.allAchievements(user);
}

Ladder ServerDatabase::getLadder()
{
	std::unique_lock<std::mutex> lock {_dbAccess};
	sqlite3_reset(_ladderStmt);

	Ladder ladder(countAccounts());

	for(size_t i = 0; i < ladder.size() && sqliteThrowExcept(sqlite3_step(_ladderStmt)) == SQLITE_ROW; ++i)
	{
		ladder.at(i).name = reinterpret_cast<const char *>(sqlite3_column_text(_ladderStmt, 0));
		ladder.at(i).victories = sqlite3_column_int(_ladderStmt, 1);
		ladder.at(i).defeats = sqlite3_column_int(_ladderStmt, 2);
	}

	return ladder;
}
int ServerDatabase::getRequired(AchievementId achievement)
{
	sqlite3_reset(_getRequiredStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_getRequiredStmt, 1, achievement));

	assert(sqliteThrowExcept(sqlite3_step(_getRequiredStmt)) == SQLITE_ROW);

	return sqlite3_column_int(_getRequiredStmt, 0);
}

bool ServerDatabase::wasNotified(UserId user, AchievementId achievement)
{
	sqlite3_reset(_wasNotifiedStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_wasNotifiedStmt, 1, user));
	sqliteThrowExcept(sqlite3_bind_int64(_wasNotifiedStmt, 2, achievement));

	return sqliteThrowExcept(sqlite3_step(_wasNotifiedStmt)) == SQLITE_ROW;
}

void ServerDatabase::setNotified(UserId user, AchievementId achievement)
{
	sqlite3_reset(_setNotifiedStmt);
	sqliteThrowExcept(sqlite3_bind_int64(_setNotifiedStmt, 1, user));
	sqliteThrowExcept(sqlite3_bind_int64(_setNotifiedStmt, 2, achievement));

	assert(sqliteThrowExcept(sqlite3_step(_setNotifiedStmt)) == SQLITE_DONE);
}

int ServerDatabase::getTimeSpent(UserId user)
{
	return getAchievementProgress(user, _getTimeSpentStmt);
}

void ServerDatabase::addTimeSpent(UserId user, int seconds)
{
	addToAchievementProgress(user, seconds, _addTimeSpentStmt);
}

int ServerDatabase::getVictories(UserId user)
{
	return getAchievementProgress(user, _getVictoriesStmt);
}

void ServerDatabase::addVictories(UserId user, int victories)
{
	addToAchievementProgress(user, victories, _addVictoriesStmt);
}

int ServerDatabase::getVictoriesInARow(UserId user)
{
	return getAchievementProgress(user, _getVictoriesInARowStmt);
}

void ServerDatabase::addVictoriesInTheCurrentRow(UserId user, int victories)
{
	sqlite3_reset(_addVictoriesInTheCurrentRowStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addVictoriesInTheCurrentRowStmt, 1, victories));
	sqliteThrowExcept(sqlite3_bind_int64(_addVictoriesInTheCurrentRowStmt, 2, user));

	assert(sqliteThrowExcept(sqlite3_step(_addVictoriesInTheCurrentRowStmt)) == SQLITE_DONE);
}

int ServerDatabase::getWithInDaClub(UserId user)
{
	return getAchievementProgress(user, _getWithInDaClubStmt);
}

void ServerDatabase::addWithInDaClub(UserId user, int withInDaClub)
{
	addToAchievementProgress(user, withInDaClub, _addWithInDaClubStmt);
}

int ServerDatabase::getRagequits(UserId user)
{
	return getAchievementProgress(user, _getRagequitsStmt);
}

void ServerDatabase::addRagequits(UserId user, int ragequits)
{
	addToAchievementProgress(user, ragequits, _addRagequitsStmt);
}

int ServerDatabase::ownAllCards(UserId user)
{
	return getAchievementProgress(user, _ownAllCardsStmt);
}

int ServerDatabase::getLadderPositionPercent(UserId user)
{
	///\TODO: Can we add UserId to LadderEntry ?
	Ladder ladder(getLadder());
	std::string name(getLogin(user));
	UserId position;

	for(position = 0; ladder.at(static_cast<std::size_t>(position)).name != name; ++position)
	{}

	return static_cast<int>((countAccounts() - position) * 100 / countAccounts());
}

int ServerDatabase::getBestLadderPositionPercent(UserId user)
{
	return getAchievementProgress(user, _getBestLadderPositionPercentStmt);
}

void ServerDatabase::updateBestLadderPositionPercent(UserId user, int playerWon)
{
	if(playerWon)
	{
		int current(getLadderPositionPercent(user));

		if(current > getBestLadderPositionPercent(user))
		{
			addToAchievementProgress(user, current, _setBestLadderPositionPercent);
		}
	}
}

int ServerDatabase::getSameCardCounter(UserId user)
{
	int counter(0);

	try
	{
		counter = getAchievementProgress(user, _getSameCardCounterStmt);
	}
	catch(const std::runtime_error& e)
	{
		// The user does not earn any card
		// so the progress is 0
	}

	return counter;
}

int ServerDatabase::getStartsInARow(UserId user)
{
	return getAchievementProgress(user, _getStartsInARowStmt);
}

void ServerDatabase::addStartsInTheCurrentRow(UserId user, int starts)
{
	///\TODO remove code duplications (...InARow achievements)
	sqlite3_reset(_addStartsInTheCurrentRowStmt);
	sqliteThrowExcept(sqlite3_bind_int(_addStartsInTheCurrentRowStmt, 1, starts));
	sqliteThrowExcept(sqlite3_bind_int64(_addStartsInTheCurrentRowStmt, 2, user));

	assert(sqliteThrowExcept(sqlite3_step(_addStartsInTheCurrentRowStmt)) == SQLITE_DONE);
}

int ServerDatabase::getDaysInARow(UserId user)
{
	return getAchievementProgress(user, _getDaysInARowStmt);
}

void ServerDatabase::addPerfectWin(UserId user, int remainingHealth)
{
	if (remainingHealth == Player::getMaxHealth())
		addToAchievementProgress(user, 1, _addPerfectWinsStmt);
}

int ServerDatabase::getPerfectWins(UserId user)
{
	return getAchievementProgress(user, _getPerfectWinsStmt);
}

void ServerDatabase::addCloseWin(UserId user, int remainingHealth)
{
	if (remainingHealth == 1)
		addToAchievementProgress(user, 1, _addCloseWinsStmt);
}

int ServerDatabase::getCloseWins(UserId user)
{
	return getAchievementProgress(user, _getCloseWinsStmt);
}


int ServerDatabase::getAchievementProgress(UserId user, sqlite3_stmt* stmt)
{
	sqlite3_reset(stmt);
	sqliteThrowExcept(sqlite3_bind_int64(stmt, 1, user));

	if(sqliteThrowExcept(sqlite3_step(stmt)) != SQLITE_ROW)
		// Should use dedicated error class
		throw std::runtime_error(std::string("getAchievementProgress: no data retreived - user: ")
		                         + std::to_string(user));

	return sqlite3_column_int(stmt, 0);
}

void ServerDatabase::addToAchievementProgress(UserId user, int value, sqlite3_stmt* stmt)
{
	sqlite3_reset(stmt);
	sqliteThrowExcept(sqlite3_bind_int(stmt, 1, value));
	sqliteThrowExcept(sqlite3_bind_int64(stmt, 2, user));

	assert(sqliteThrowExcept(sqlite3_step(stmt)) == SQLITE_DONE);
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



ServerDatabase::AchievementManager::AchievementManager(ServerDatabase& database) :
	_database(database)
{

}

AchievementList ServerDatabase::AchievementManager::newAchievements(const PostGameData& postGame, UserId user)
{
	AchievementList achievements;

	for(size_t i = 0; i < _achievementsList.size(); ++i)
	{
		// update
		if(_achievementsList[i].addMethod != nullptr)
			(_database.*(_achievementsList[i].addMethod))(user, postGame.*(_achievementsList[i].toAddValue));

		// get new value
		int currentProgress = (_database.*(_achievementsList[i].getMethod))(user);

		if(!_database.wasNotified(user, _achievementsList[i].id)
		        && currentProgress >= _database.getRequired(_achievementsList[i].id))
		{
			_database.setNotified(user, _achievementsList[i].id);
			achievements.emplace_back(Achievement {_achievementsList[i].id, currentProgress});
		}
	}

	return achievements;
}

AchievementList ServerDatabase::AchievementManager::allAchievements(UserId user)
{
	AchievementList achievements;

	for(size_t i = 0; i < _achievementsList.size(); ++i)
		achievements.emplace_back(Achievement {_achievementsList[i].id, (_database.*(_achievementsList[i].getMethod))(user)});

	return achievements;
}
