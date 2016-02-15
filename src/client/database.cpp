#include "../../include/client/database.hpp"

/*
 * Database schema
 *
 * CREATE TABLE Card (
 *   id INTEGER PRIMARY KEY ASC AUTOINCREMENT,
 *   name TEXT UNIQUE NOT NULL,
 *   description TEXT NOT NULL,
 *   effect INTEGER NOT NULL, -- used as effects[difference_type]
 *   -- parametersListSize INTEGER, -- not stored because it is sizeof(parameters)/sizeof(int32_t)
 *   parameters BLOB, -- int[]
 *   cost INTEGER
 * );
 *
 * CREATE TABLE Monster (
 *   id UNIQUE NOT NULL,
 *   health INTEGER NOT NULL CHECK (health >= 0),
 *   attack INTEGER NOT NULL CHECK (attack >= 0),
 *     FOREIGN KEY(id) REFERENCES Card(id)
 * );
 * 
 */

DatabaseClient::DatabaseClient(std::string filename) : Database(filename) {}

