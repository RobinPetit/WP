#ifndef _SERVER_DATABASE_SERVER_HPP
#define _SERVER_DATABASE_SERVER_HPP

// std-C++ headers
#include <array>
#include <map>
#include <memory>
// WizardPoker headers
#include "common/Database.hpp"
#include "common/Identifiers.hpp"
#include "common/Card.hpp"
#include "common/Achievement.hpp"
#include "server/ServerCardData.hpp"
#include "server/PostGameData.hpp"

class Player;
// Cards
class Creature;
class Spell;

// for now login and name are used for the same string

/// Interface to the server database.
/// In comments I will use Card Template to talk about a card 'out-game'
/// and Card to talk about a card 'in-game', with a owner...
class ServerDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ServerDatabase(const std::string& filename = FILENAME);

	//////////////// Cards
	/// Card* to be deallocated by the caller
	Card* getCard(CardId card, Player& player);
	const CommonCardData* getCardData(CardId card);
	/// Number of card templates in database
	CardId countCards();
	/// Get a valid id of a card template
	CardId getRandomCardId();

	//////////////// Users
	UserId getUserId(const std::string& login);
	std::string getLogin(UserId id);
	bool areIdentifiersValid(const std::string& login, const std::string& password);
	bool isRegistered(const std::string& login);
	void registerUser(const std::string& login, const std::string& password);

	unsigned countAccounts();

	//////////////// Friends
	inline FriendsList getFriendsList(UserId id)
	{
		return getAnyFriendsList(id, _friendListStmt);
	}
	inline FriendsList getFriendshipRequests(UserId id)
	{
		return getAnyFriendsList(id, _friendshipRequestsStmt);
	}

	void addFriend(UserId id1, UserId id2);
	void removeFriend(UserId id1, UserId id2);
	bool areFriend(UserId id1, UserId id2);

	void addFriendshipRequest(UserId from, UserId to);
	void removeFriendshipRequest(UserId from, UserId to);
	bool isFriendshipRequestSent(UserId from, UserId to);

	//////////////// CardCollections
	CardsCollection getCardsCollection(UserId id);
	void addCard(UserId id, CardId card);

	//////////////// Decks
	std::vector<Deck> getDecks(UserId id);
	Deck getDeckByName(UserId id, const std::string& deckName);
	void createDeck(UserId id, const Deck& deck);
	std::vector<CardId> getFirstCardIds(unsigned count); // TODO use it in Deck constructor
	void deleteDeckByName(UserId id, const std::string& deckName);
	void editDeck(UserId id, const Deck& deck); // Deck should contains the DeckId

	//////////////// Achievements
	/// Unlock new card and achievements
	AchievementList newAchievements(const PostGameData&, UserId);
	AchievementList getAchievements(UserId);
	int getWithInDaClub(UserId);
	Ladder getLadder();

	virtual ~ServerDatabase();

private:
	friend class AchievementManager;
	/// A part of the database which perform computation of achievements
	class AchievementManager
	{
		ServerDatabase& _database;

		struct AchievementsListItem
		{
			AchievementId id;
			void (ServerDatabase::*addMethod)(UserId, int);
			const int PostGameData::*toAddValue;
			int (ServerDatabase::*getMethod)(UserId);
		};
		std::array<AchievementsListItem, 12> _achievementsList
		{
			{
				AchievementsListItem {
					1,
					&ServerDatabase::addTimeSpent,
					&PostGameData::gameDuration,
					&ServerDatabase::getTimeSpent
				},
				AchievementsListItem {
					2,
					&ServerDatabase::addVictories,
					&PostGameData::playerWon,
					&ServerDatabase::getVictories
				},
				AchievementsListItem {
					3,
					&ServerDatabase::addVictoriesInTheCurrentRow,
					&PostGameData::playerWon,
					&ServerDatabase::getVictoriesInARow
				},
				AchievementsListItem {
					4,
					&ServerDatabase::addWithInDaClub,
					&PostGameData::opponentInDaClub,
					&ServerDatabase::getWithInDaClub
				},
				AchievementsListItem {
					5,
					&ServerDatabase::addRagequits,
					&PostGameData::playerRageQuit,
					&ServerDatabase::getRagequits
				},
				AchievementsListItem {
					6,
					nullptr, nullptr,
					&ServerDatabase::ownAllCards
				},
				AchievementsListItem {
					///\TODO: the current implementation of this achievement is HEAVY
					7,
					&ServerDatabase::updateBestLadderPositionPercent,
					&PostGameData::playerWon,
					&ServerDatabase::getBestLadderPositionPercent
				},
				AchievementsListItem {
					8,
					nullptr, nullptr,
					&ServerDatabase::getSameCardCounter
				},
				AchievementsListItem {
					9,
					&ServerDatabase::addStartsInTheCurrentRow,
					&PostGameData::playerStarted,
					&ServerDatabase::getStartsInARow
				},
				AchievementsListItem {
					10,
					nullptr, nullptr,
					&ServerDatabase::getDaysInARow
				},
				AchievementsListItem {
					11,
					&ServerDatabase::addPerfectWin,
					&PostGameData::remainingHealth,
					&ServerDatabase::getPerfectWins
				},
				AchievementsListItem {
					12,
					&ServerDatabase::addCloseWin,
					&PostGameData::remainingHealth,
					&ServerDatabase::getCloseWins
				}
			}
		};

	public:
		AchievementManager(ServerDatabase&);
		///\TODO For now newAchievements update lastDayPlayed information but I think it is not his purpose
		/// Give a new card if game won and unlock others achievements (and update lastDayPlayed information)
		AchievementList newAchievements(const PostGameData&, UserId);
		AchievementList allAchievements(UserId);
	};

	/// Default relative path to sqlite3 file
	static const char FILENAME[];
	std::map<const CardId, const std::unique_ptr<const CommonCardData> > _cardData;
	AchievementManager _achievementManager;

	/// Used by getFriendsList and getAnyFriendsList
	FriendsList getAnyFriendsList(UserId id, sqlite3_stmt * stmt);

	/// Add a card to _cards (used by contructor)
	void createSpellData();
	/// Add a card to _cards (used by contructor)
	void createCreatureData();
	/// Used by createSpellData() and createCreatureData()
	std::vector<EffectParamsCollection> createCardEffects(CardId id);

	//////////////// Achievements
	// this methods should be used only by the nested class AchievementManager
	// so I put this in private and declare AchievementManager (which is usable only by the ServerDatabase class)
	// as a friend
	int getRequired(AchievementId);
	bool wasNotified(UserId, AchievementId);
	void setNotified(UserId, AchievementId);

	int getAchievementProgress(UserId id, sqlite3_stmt * stmt);
	int getTimeSpent(UserId);
	int getVictories(UserId);
	int getVictoriesInARow(UserId);
	int getRagequits(UserId);
	int ownAllCards(UserId);
	///\except std::out_of_range if UserId doesnt exists
	int getLadderPositionPercent(UserId);
	int getBestLadderPositionPercent(UserId);
	int getSameCardCounter(UserId);
	int getStartsInARow(UserId);
	int getDaysInARow(UserId);
	int getPerfectWins(UserId);
	int getCloseWins(UserId);

	void addToAchievementProgress(UserId id, int value, sqlite3_stmt * stmt);
	void addTimeSpent(UserId, int seconds);
	void addVictories(UserId, int victories);
	void addVictoriesInTheCurrentRow(UserId, int victories);
	void addWithInDaClub(UserId, int withInDaClub);
	void addRagequits(UserId, int ragequits);
	void addStartsInTheCurrentRow(UserId, int starts);
	void updateBestLadderPositionPercent(UserId, int playerWon); ///\TODO playerWon should be bool
	/// add 1 to perfectWins if remainingHealth is max
	/// (This is note the same way that other addAchievement methods, note that is addPerfectWin and not addPerfectWins)
	void addPerfectWin(UserId, int remainingHealth);
	/// add 1 to closeWins if remainingHealth is 1
	void addCloseWin(UserId, int remainingHealth);

	sqlite3_stmt * _friendListStmt;
	sqlite3_stmt * _UserIdStmt;
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
	sqlite3_stmt * _getVictoriesInARowStmt;
	sqlite3_stmt * _getWithInDaClubStmt;
	sqlite3_stmt * _getRagequitsStmt;
	sqlite3_stmt * _ownAllCardsStmt;
	sqlite3_stmt * _getBestLadderPositionPercentStmt;
	sqlite3_stmt * _getSameCardCounterStmt;
	sqlite3_stmt * _getStartsInARowStmt;
	sqlite3_stmt * _getDaysInARowStmt;
	sqlite3_stmt * _getPerfectWinsStmt;
	sqlite3_stmt * _getCloseWinsStmt;

	sqlite3_stmt * _addTimeSpentStmt;
	sqlite3_stmt * _addVictoriesStmt;
	sqlite3_stmt * _addVictoriesInTheCurrentRowStmt;
	sqlite3_stmt * _addWithInDaClubStmt;
	sqlite3_stmt * _addRagequitsStmt;
	sqlite3_stmt * _addStartsInTheCurrentRowStmt;
	sqlite3_stmt * _updateLastDayPlayedStmt;
	sqlite3_stmt * _setBestLadderPositionPercent;
	sqlite3_stmt * _addPerfectWinsStmt;
	sqlite3_stmt * _addCloseWinsStmt;

	// `constexpr std::array::size_type size() const;`
	// -> future uses have to be _statements.size() -> 49 is written only one time
	StatementsList<51> _statements
	{
		{
			Statement {
				&_UserIdStmt,
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
				"	FROM Account "
				"	ORDER BY (victories+1)/(defeats+1) DESC;"

			},
			Statement {
				&_getTimeSpentStmt,
				"SELECT secondsSpentPlaying "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_getVictoriesStmt,
				"SELECT victories "
				"	FROM Account "
				"	WHERE id == ?1;"
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
			},
			Statement {
				&_getVictoriesInARowStmt,
				"SELECT maxVictoriesInARow "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_addVictoriesInTheCurrentRowStmt,
				"UPDATE Account "
				"	SET currentVictoriesInARow = currentVictoriesInARow + ?1 "
				"	WHERE id == ?2;"
			},
			Statement {
				&_getWithInDaClubStmt,
				"SELECT gameWithInDaClub "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement { // 36
				&_addWithInDaClubStmt,
				"UPDATE Account "
				"	SET gameWithInDaClub = gameWithInDaClub + ?1 "
				"	WHERE id == ?2;"
			},
			Statement {
				&_getRagequitsStmt,
				"SELECT ragequits "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_addRagequitsStmt,
				"UPDATE Account "
				"	SET ragequits = ragequits + ?1 "
				"	WHERE id == ?2;"
			},
			Statement {
				///\TODO I'm not yet familiar with aggregations
				&_ownAllCardsStmt,
				"SELECT min(Card.id IN (SELECT GivenCard.card FROM GivenCard WHERE owner == ?1)) "
				"	FROM Card;"
			},
			Statement { // 40
				&_getBestLadderPositionPercentStmt,
				"SELECT bestLadderPositionPercent "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_getSameCardCounterStmt,
				"SELECT COUNT(*) AS counter "
				"	FROM GivenCard "
				"	WHERE owner == ?1 "
				"	GROUP BY card "
				"	ORDER BY counter DESC "
				"	LIMIT 1;"
			},
			Statement {
				&_getStartsInARowStmt,
				"SELECT maxStartsInARow "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_addStartsInTheCurrentRowStmt,
				"UPDATE Account "
				"	SET currentStartsInARow = currentStartsInARow + ?1 "
				"	WHERE id == ?2;"
			},
			Statement { // 44
				&_getDaysInARowStmt,
				"SELECT maxDaysPlayedInARow "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_updateLastDayPlayedStmt,
				"UPDATE Account "
				"	SET lastDayPlayed = round(julianday('now')) "
				"	WHERE id == ?1;"
			},
			Statement {
				&_setBestLadderPositionPercent,
				"UPDATE Account "
				"	SET bestLadderPositionPercent = ?1 "
				"	WHERE id == ?2;"
			},
			Statement {
				&_getPerfectWinsStmt,
				"SELECT perfectWins "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement { // 48
				&_addPerfectWinsStmt,
				"UPDATE Account "
				"	SET perfectWins = perfectWins + ?1 "
				"	WHERE id == ?2;"
			},
			Statement {
				&_getCloseWinsStmt,
				"SELECT closeWins "
				"	FROM Account "
				"	WHERE id == ?1;"
			},
			Statement {
				&_addCloseWinsStmt,
				"UPDATE Account "
				"	SET closeWins = closeWins + ?1 "
				"	WHERE id == ?2;"
			}
		}
	};
};

#endif //_DATABASE_SERVER_HPP
