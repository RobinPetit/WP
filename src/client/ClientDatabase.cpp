#include "client/ClientDatabase.hpp"

const char ClientDatabase::FILENAME[] = "../resources/client/database.db";
ClientDatabase::ClientDatabase(const std::string& filename) : Database(filename)
{}
{
	prepareStmt(_getCardFullStmt);
}

ClientDatabase::~ClientDatabase()
{
	int errcode;

	if((errcode = sqlite3_finalize(_getCardStmt)) != SQLITE_OK)
		std::cerr << "ERROR while finalizing statement "
		          << ": " << sqlite3_errstr(errcode)
		          << std::endl;

}
