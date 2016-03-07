#include "common/Database.hpp"

#include <string>
#include <cstring>

Database::Database(std::string filename)
{
	std::cout << filename.c_str() << std::endl;
	sqliteThrowExcept(sqlite3_open(filename.c_str(), &_database));
	sqliteThrowExcept(sqlite3_exec(_database, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr));
}

void Database::prepareStmt(Statement& statement)
{
	sqliteThrowExcept(sqlite3_prepare_v2(_database, statement.query(), std::strlen(statement.query()),
	                                     statement.statement(), nullptr));
}

Database::~Database()
{
	// TODO finalize all prepared statements, close all BLOB handles, and finish all sqlite3_backup objects associated with the sqlite3 object
	// It is done but think about it when modify the class
	if(sqlite3_close(_database) != SQLITE_OK)
		std::cerr << "ERROR while closing database connection" << std::endl; // TODO error handling
}

int Database::sqliteThrowExcept(int errcode) const
{
	switch(errcode)
	{
	case SQLITE_ROW:
	case SQLITE_DONE:
	case SQLITE_OK:
		break;

	case SQLITE_CORRUPT:
		throw std::runtime_error("Database corrupted, need reinstall");

	case SQLITE_INTERNAL:
		throw std::runtime_error("Database engine unexpected error");

	case SQLITE_PERM:
	case SQLITE_READONLY:
		throw std::runtime_error("Database permissions error");

	case SQLITE_ABORT:
	case SQLITE_INTERRUPT:
		throw std::runtime_error("Database operation aborted");

	case SQLITE_BUSY:
	case SQLITE_LOCKED:
		throw std::runtime_error("Database busy/locked: is this instance the only one running?");

	case SQLITE_NOMEM:
	case SQLITE_IOERR:
	case SQLITE_FULL:
		throw std::runtime_error("IO error");

	case SQLITE_CANTOPEN:
		throw std::runtime_error("Database MS unable to open file");

	case SQLITE_SCHEMA:
		throw std::runtime_error("Database schema has changed, need reinstall or update");

	case SQLITE_ERROR:
	default:
		throw std::runtime_error(std::string(sqlite3_errstr(errcode)) + " (" + std::to_string(errcode) + ")");
	}

	return errcode;
}

