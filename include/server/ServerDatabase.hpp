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
	std::vector<Deck> getDecks(const int userId);
	CardsCollection getCardsCollection(const int userId);

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

	// `constexpr std::array::size_type size() const;`
	// -> I consider this 4 as the definition of the variable, so it is not a magic number
	// -> future uses have to be _statements.size() -> 4 is writed only one time
	StatementsList<6> _statements
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
				"FROM Friendship INNER JOIN Account ON second == id "
				"WHERE first == ?1;"
			},
			Statement {
				&_friendshipRequestsStmt,
				"WITH FriendRequests(from_) AS (SELECT from_ FROM FriendRequest WHERE to_ == ?1) "
				"SELECT from_ AS id, login AS name "
				"FROM FriendRequests INNER JOIN Account ON from_ == id;"
			},
			Statement {
				&_decksStmt,
				"SELECT name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9, "
				"Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19 "
				"FROM Deck WHERE Owner == ?1;"
			},
			Statement {
				&_cardsCollectionStmt,
				"SELECT card "
				"FROM GivenCard "
				"WHERE owner == ?1 "
				"ORDER BY card;"
			}
		}
	};
};

#endif //_DATABASE_SERVER_HPP
