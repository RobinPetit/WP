#ifndef _SERVER_DATABASE_SERVER_HPP
#define _SERVER_DATABASE_SERVER_HPP

#include <map>
#include <memory>

#include "common/Database.hpp"
#include "common/Identifiers.hpp"
#include "common/Card.hpp"
#include "server/ServerCardData.hpp"

class Player;
class Creature;
class Spell;

// for the moment login and name are used for the same string

/// Interface to the server database.
class ServerDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ServerDatabase(const std::string& filename = FILENAME);

	Card* getCard(cardId card, Player& player);
	const CommonCardData* getCardData(cardId card);
	/// Number of card templates in database
	cardId countCards();
	cardId getRandomCardId();

	userId getUserId(const std::string& login);
	std::string getLogin(userId id);

	inline FriendsList getFriendsList(userId id)
	{
		return getAnyFriendsList(id, _friendListStmt);
	}
	inline FriendsList getFriendshipRequests(userId id)
	{
		return getAnyFriendsList(id, _friendshipRequestsStmt);
	}

	void addFriend(userId id1, userId id2);
	void removeFriend(userId id1, userId id2);
	bool areFriend(userId id1, userId id2);

	void addFriendshipRequest(userId from, userId to);
	void removeFriendshipRequest(userId from, userId to);
	bool isFriendshipRequestSent(userId from, userId to);

	CardsCollection getCardsCollection(userId id);
	void addCard(userId id, cardId card);

	std::vector<Deck> getDecks(userId id);
	Deck getDeckByName(userId id, const std::string& deckName);
	void createDeck(userId id, const Deck& deck);
	std::vector<cardId> getFirstCardIds(unsigned count); // TODO use it in Deck constructor
	void deleteDeckByName(userId id, const std::string& deckName);
	void editDeck(userId id, const Deck& deck); // Deck should contains the deckId

	bool areIdentifiersValid(const std::string& login, const std::string& password);
	bool isRegistered(const std::string& login);
	void registerUser(const std::string& login, const std::string& password);

	unsigned countAccounts();

	// Achievements
	int getRequired(AchievementId);
	bool wasNotified(userId, AchievementId);
	void setNotified(userId, AchievementId);

	Ladder getLadder();
	int getTimeSpent(userId);
	int getVictories(userId);

	void addTimeSpent(userId, int seconds);
	void addVictories(userId, int victories);

	virtual ~ServerDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];
	std::map<const cardId, const std::unique_ptr<const CommonCardData> > _cardData;

	FriendsList getAnyFriendsList(userId id, sqlite3_stmt * stmt);
	/// Add a card to _cards;
	void createSpellData();
	void createCreatureData();
	std::vector<EffectParamsCollection> createCardEffects(cardId id);
	// Achievements
	int getAchievementProgress(userId id, sqlite3_stmt * stmt);
	void addToAchievementProgress(userId id, int value, sqlite3_stmt * stmt);

	sqlite3_stmt * _friendListStmt;
	sqlite3_stmt * _userIdStmt;
	sqlite3_stmt * _loginStmt;
	sqlite3_stmt * _friendshipRequestsStmt;
	sqlite3_stmt * _decksStmt;
	sqlite3_stmt * _cardsCollectionStmt;
	sqlite3_stmt * _addFriendStmt;
	sqlite3_stmt * _removeFriendStmt;
	sqlite3_stmt * _areFriendStmt;
	sqlite3_stmt * _addFriendshipRequestStmt;
	sqlite3_stmt * _removeFriendshipRequestStmt;
	sqlite3_stmt * _isFriendshipRequestSentStmt;
	sqlite3_stmt * _registerUserStmt;
	sqlite3_stmt * _areIdentifiersValidStmt;
	sqlite3_stmt * _createDeckStmt;
	sqlite3_stmt * _deleteDeckByNameStmt;
	sqlite3_stmt * _editDeckByNameStmt;
	sqlite3_stmt * _getSpellCardsStmt;
	sqlite3_stmt * _getCreatureCardsStmt;
	sqlite3_stmt * _getCardEffectsStmt;
	sqlite3_stmt * _newCardStmt;
	sqlite3_stmt * _countAccountsStmt;
	sqlite3_stmt * _getFirstCardIdsStmt;
	sqlite3_stmt * _countCardsStmt;
	sqlite3_stmt * _getRandomCardIdStmt;
	// achievements
	sqlite3_stmt * _getRequiredStmt;
	sqlite3_stmt * _wasNotifiedStmt;
	sqlite3_stmt * _setNotifiedStmt;

	sqlite3_stmt * _ladderStmt;
	sqlite3_stmt * _getTimeSpentStmt;
	sqlite3_stmt * _getVictoriesStmt;

	sqlite3_stmt * _addTimeSpentStmt;
	sqlite3_stmt * _addVictoriesStmt;

	// `constexpr std::array::size_type size() const;`
	// -> future uses have to be _statements.size() -> 33 is written only one time
	StatementsList<33> _statements
	{
		{
			Statement {
				&_userIdStmt,
				"SELECT id FROM Account WHERE login == ?1;"
			},
			Statement {
				&_loginStmt,
				"SELECT login FROM Account WHERE id == ?1;"
			},
			Statement {
				&_friendListStmt,
				"SELECT id,login "
				"	FROM Friendship INNER JOIN Account ON second == id "
				"	WHERE first == ?1;"
			},
			Statement {
				&_friendshipRequestsStmt,
				/*"WITH FriendRequests(from_) AS (SELECT from_ FROM FriendRequest WHERE to_ == ?1) "
				"SELECT from_ AS id, login AS name "
				"	FROM FriendRequests INNER JOIN Account ON from_ == id;"*/ // Bug on some platforms/configs (*32?)
				"SELECT from_ AS id, login AS name "
				"	FROM FriendRequest INNER JOIN Account ON from_ == id WHERE to_ == ?1;"
			},
			Statement { // 4
				&_decksStmt,
				"SELECT name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9, "
				"		Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19 "
				"	FROM Deck WHERE Owner == ?1;"
			},
			Statement {
				&_cardsCollectionStmt,
				"SELECT card "
				"	FROM GivenCard "
				"	WHERE owner == ?1 "
				"	ORDER BY card;"
			},
			Statement {
				&_addFriendStmt,
				"INSERT INTO Friend "
				"	VALUES(?1,?2);" // TRIGGER addFriend will remove obselete friendshipRequests
			},
			Statement {
				&_removeFriendStmt,
				"DELETE FROM Friend "
				"	WHERE(first == ?1 AND second == ?2);" // With ?1 < ?2. See initdatabase.sql for reason
			},
			Statement { // 8
				&_areFriendStmt,
				"SELECT 1 FROM Friendship "
				"	WHERE(first == ?1 AND second == ?2);"
			},
			Statement {
				&_addFriendshipRequestStmt,
				"INSERT INTO FriendRequest(from_, to_) "
				"	VALUES(?1,?2);"
			},
			Statement {
				&_removeFriendshipRequestStmt,
				"DELETE FROM FriendRequest "
				"	WHERE from_ == ?1 AND to_ == ?2;"
			},
			Statement {
				&_isFriendshipRequestSentStmt,
				"SELECT 1 FROM FriendRequest "
				"	WHERE from_ == ?1 AND to_ == ?2;"
			},
			Statement { // 12
				&_registerUserStmt,
				"INSERT INTO Account(login, password) "
				"	VALUES(?1,?2);"
			},
			Statement {
				&_areIdentifiersValidStmt,
				"SELECT 1 FROM Account "
				"	WHERE(login == ?1 and password == ?2);"
			},
			Statement {
				&_createDeckStmt,
				"INSERT INTO Deck(owner, name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9, "
				"		Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19) "
				"	VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, "
				"		?13, ?14, ?15, ?16, ?17, ?18, ?19, ?20, ?21, ?22);"
			},
			Statement {
				&_deleteDeckByNameStmt,
				"DELETE FROM Deck "
				"	WHERE owner == ?1 and name == ?2;"
			},
			Statement { // 16
				&_editDeckByNameStmt,
				"UPDATE Deck "
				"	SET card0 = ?2, card1 = ?3, card2 = ?4, card3 = ?5, card4 = ?6, card5 = ?7, card6 = ?8, "
				"		card7 = ?9, card8 = ?10, card9 = ?11, card10 = ?12, card11 = ?13, card12 = ?14, card13 = ?15, "
				"		card14 = ?16, card15 = ?17, card16 = ?18, card17 = ?19, card18 = ?20, card19 = ?21 "
				"	WHERE owner == ?22 AND name == ?1;" // name <- ?1 because complete query should be `...SET name = ?1...`
			},
			Statement {
				&_getSpellCardsStmt,
				"SELECT id, cost FROM SpellCard;"
			},
			Statement {
				&_getCreatureCardsStmt,
				"SELECT id, cost, attack, health, shield, shieldType FROM CreatureCard;"
			},
			Statement {
				&_getCardEffectsStmt,
				"SELECT parameter0, parameter1, parameter2, parameter3,"
				"	parameter4, parameter5, parameter6 "
				"FROM Effect WHERE owner == ?1;"
			},
			Statement { // 20
				&_newCardStmt,
				"INSERT INTO GivenCard(card, owner) "
				"	VALUES (?1, ?2);"
			},
			Statement {
				&_getFirstCardIdsStmt,
				"SELECT id "
				"	FROM FullCard "
				"	ORDER BY id "
				"	LIMIT ?1;"
			},
			Statement {
				&_countCardsStmt,
				"SELECT count() FROM FullCard;"
			},
			Statement {
				&_getRandomCardIdStmt,
				"SELECT id FROM FullCard ORDER BY random() LIMIT 1;"
			},
			Statement { // 24
				&_countAccountsStmt,
				"SELECT COUNT (*) FROM Account;"
			},
			Statement {
				&_getRequiredStmt,
				"SELECT progressRequired "
				"	FROM Achievement "
				"	WHERE id == ?1;"
			},
			Statement {
				&_wasNotifiedStmt,
				"SELECT 1 FROM NotifiedAchievement "
				"	WHERE owner == ?1 and achievement == ?2;"
			},
			Statement {
				&_setNotifiedStmt,
				"INSERT INTO NotifiedAchievement(owner, achievement) " ///\TODO use INSERT OR IGNORE (I use INSERT for now to detect errors)
				"	VALUES(?1, ?2);"
			},
			Statement { // 28
				&_ladderStmt,
				"SELECT login, victories, defeats "
				"	FROM Account;"
			},
			Statement {
				&_getTimeSpentStmt,
				"SELECT secondsSpentPlaying "
				"	FROM Account "
				"	WHERE id == ?1"
			},
			Statement {
				&_getVictoriesStmt,
				"SELECT victories "
				"	FROM Account "
				"	WHERE id == ?1"
			},
			Statement {
				&_addTimeSpentStmt,
				"UPDATE Account "
				"	SET secondsSpentPlaying = secondsSpentPlaying + ?1 "
				"	WHERE id == ?2;"
			},
			Statement { // 32
				&_addVictoriesStmt,
				"UPDATE Account "
				"	SET victories = victories + ?1 "
				"	WHERE id == ?2;"
			}
		}
	};
};

#endif //_DATABASE_SERVER_HPP
