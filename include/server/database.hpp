#ifndef _DATABASE_SERVER_HPP
#define _DATABASE_SERVER_HPP

#include "../common/database.hpp"

/// Interface to the server database.
class DatabaseServer : public Database
{
    /// Default relative path to sqlite3 file
    static constexpr char FILENAME[] = "../ressources/server/database.db";
public:
    /// Constructor
    /// \param filename: relative path to sqlite3 file.
    explicit DatabaseServer(std::string filename = FILENAME);
};

#endif //_DATABASE_SERVER_HPP
