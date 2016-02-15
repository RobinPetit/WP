#include "../../include/common/database.hpp"

#include <string>

Database::Database(std::string filename)
{
    const std::string filenamePrefix = "file:";

    _sqliteErrorToExcept(sqlite3_open((filenamePrefix + filename).c_str(), &_ppDb));
}

Database::~Database()
{
    // TODO finalize all prepared statements, close all BLOB handles, and finish all sqlite3_backup objects associated with the sqlite3 object
    if (sqlite3_close(_ppDb) != SQLITE_OK)
        std::cerr << "ERROR while closing database connection" << std::endl; // TODO error handling
}

void Database::_sqliteErrorToExcept(int errcode) const
{
    switch (errcode) {
        case SQLITE_OK:
            break;
        case SQLITE_CORRUPT:
            throw std::runtime_error("Database corrupted, need reinstall");
            break;
        case SQLITE_INTERNAL:
            throw std::runtime_error("Database engine unexpected error");
            break;
        case SQLITE_PERM:
        case SQLITE_READONLY:
            throw std::runtime_error("Database permissions error");
            break;
        case SQLITE_ABORT:
        case SQLITE_INTERRUPT:
            throw std::runtime_error("Database operation aborted");
            break;
        case SQLITE_BUSY:
        case SQLITE_LOCKED:
            throw std::runtime_error("Database busy/locked: is this instance the only one running?");
            break;
        case SQLITE_NOMEM:
        case SQLITE_IOERR:
        case SQLITE_FULL: 
            throw std::runtime_error("IO error");
            break;
        case SQLITE_ERROR:
        default:
            throw std::runtime_error("Database unknown error"); // TODO add error code to what_arg
    }
}

