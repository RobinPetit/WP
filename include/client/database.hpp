#ifndef _DATABASE_CLIENT_HPP
#define _DATABASE_CLIENT_HPP

#include "../common/database.hpp"

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

#endif //_DATABASE_CLIENT_HPP
