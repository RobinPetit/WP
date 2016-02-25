#ifndef _CLIENT_DATABASE_CLIENT_HPP
#define _CLIENT_DATABASE_CLIENT_HPP

#include "common/Database.hpp"

/// Interface to the client database.
class DatabaseClient : public Database
{
    /// Default relative path to sqlite3 file
    static constexpr char FILENAME[] = "../ressources/client/database.db";
public:
    /// Constructor
    /// \param filename: relative path to sqlite3 file.
    explicit DatabaseClient(std::string filename = FILENAME);
};

#endif // _CLIENT_DATABASE_CLIENT_HPP
