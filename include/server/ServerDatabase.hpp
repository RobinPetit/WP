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
	FriendsList getFriendsList(const int userId);

	virtual ~ServerDatabase();

private:
	/// Default relative path to sqlite3 file
	static const char FILENAME[];

	sqlite3_stmt * _friendListStmt;
	sqlite3_stmt * _userIdStmt;
	sqlite3_stmt * _loginStmt;

	// `constexpr std::array::size_type size() const;`
	// -> I consider this 3 as the definition of the variable, so it is not a magic number
	// -> future uses have to be _statements.size() -> 3 is writed only one time
	StatementsList<3> _statements
	{
		{
			Statement {
				&_friendListStmt,
				"SELECT id,login "
				"FROM Friendship INNER JOIN Account ON second == id "
				"WHERE first == ?1;"
			},
			Statement {
				&_userIdStmt,
				"SELECT id FROM Account WHERE login == ?1;"
			},
			Statement {
				&_loginStmt,
				"SELECT login FROM Account WHERE id == ?1;"
			}
		}
	};
};

#endif //_DATABASE_SERVER_HPP
