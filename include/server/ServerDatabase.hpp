#ifndef _SERVER_DATABASE_SERVER_HPP
#define _SERVER_DATABASE_SERVER_HPP

#include "common/Database.hpp"

// for the moment login and name are used for the same string

/// Interface to the server database.
class ServerDatabase : public Database
{
public:
	/// Constructor
	/// \param filename: relative path to sqlite3 file.
	explicit ServerDatabase(const std::string filename = FILENAME);

	int getUserId(const std::string login);
	std::string getLogin(const int userId);

	inline FriendsList getFriendsList(const int userId)
	{
		return getAnyFriendsList(userId, _friendListStmt);
	}
	inline FriendsList getFriendshipRequests(const int userId)
	{
		return getAnyFriendsList(userId, _friendshipRequestsStmt);
	}

	Ladder getLadder();

	void addFriend(const int userId1, const int userId2);
	void removeFriend(const int userId1, const int userId2);
	bool areFriend(const int userId1, const int userId2);

	void addFriendshipRequest(const int from, const int to);
	void removeFriendshipRequest(const int from, const int to);
	bool isFriendshipRequestSent(const int from, const int to);

	CardsCollection getCardsCollection(const int userId);

	std::vector<Deck> getDecks(const int userId);

	bool areIdentifersValid(const std::string& login, const std::string& password);
	bool isRegistered(const std::string& login);
	void registerUser(const std::string& login, const std::string& password);

	virtual ~ServerDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];

	FriendsList getAnyFriendsList(const int userId, sqlite3_stmt * stmt);

	sqlite3_stmt * _friendListStmt;
	sqlite3_stmt * _userIdStmt;
	sqlite3_stmt * _loginStmt;
	sqlite3_stmt * _friendshipRequestsStmt;
	sqlite3_stmt * _decksStmt;
	sqlite3_stmt * _cardsCollectionStmt;
	sqlite3_stmt * _ladderStmt;
	sqlite3_stmt * _addFriendStmt;
	sqlite3_stmt * _removeFriendStmt;
	sqlite3_stmt * _areFriendStmt;
	sqlite3_stmt * _addFriendshipRequestStmt;
	sqlite3_stmt * _removeFriendshipRequestStmt;
	sqlite3_stmt * _isFriendshipRequestSentStmt;
	sqlite3_stmt * _registerUserStmt;
	sqlite3_stmt * _areIdentifersValidStmt;

	// `constexpr std::array::size_type size() const;`
	// -> I consider this 15 as the definition of the variable, so it is not a magic number
	// -> future uses have to be _statements.size() -> 15 is writed only one time
	StatementsList<15> _statements
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
				&_ladderStmt,
				"SELECT login, victories, defeats "
				"	FROM Account "
				"	ORDER BY CAST(victories AS REAL)/defeats DESC, victories DESC, givingup "
				"	LIMIT ?1;"
			},
			Statement {
				&_addFriendStmt,
				"INSERT INTO Friend "
				"	VALUES(?1,?2);" // TRIGGER addFriend will remove obselete friendshipRequests
			},
			Statement { // 8
				&_removeFriendStmt,
				"DELETE FROM Friend "
				"	WHERE(first == ?1 AND second == ?2);" // With ?1 < ?2. See initdatabase.sql for reason
			},
			Statement {
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
			Statement { // 12
				&_isFriendshipRequestSentStmt,
				"SELECT 1 FROM FriendRequest "
				"	WHERE from_ == ?1 AND to_ == ?2;"
			},
			Statement {
				&_registerUserStmt,
				"INSERT INTO Account(login, password) "
				"	VALUES(?1,?2);"
			},
			Statement {
				&_areIdentifersValidStmt,
				"SELECT 1 FROM Account "
				"	WHERE(login == ?1 and password == ?2);"
			}
		}
	};
};

#endif //_DATABASE_SERVER_HPP
