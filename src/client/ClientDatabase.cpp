#include "client/ClientDatabase.hpp"


const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
ClientDatabase::ClientDatabase(std::string filename) : Database(filename)
{}
