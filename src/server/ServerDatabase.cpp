#include "server/ServerDatabase.hpp"

ServerDatabase::ServerDatabase(std::string filename) : Database(filename) {}

const char ServerDatabase::FILENAME[] = "../resources/server/database.db";
