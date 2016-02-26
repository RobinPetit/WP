#include "client/ClientDatabase.hpp"

const char ClientDatabase::FILENAME[] = "../resources/client/database.db";
ClientDatabase::ClientDatabase(std::string filename) : Database(filename) {}
