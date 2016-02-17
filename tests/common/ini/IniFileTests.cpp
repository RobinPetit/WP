#include <fstream>
#include <cstdio>
#include <boost/test/unit_test.hpp>
#include "common/ini/IniFile.hpp"
#include "common/ErrorCode.hpp"

struct IniFileTestsFixture
{
	static const std::string fileContent;
	static const std::string fileName;
	IniFile cfg;
	IniFileTestsFixture():
		cfg()
	{

		std::ofstream file(fileName);
		file << std::string(fileContent);
		file.close();
	}

	~IniFileTestsFixture()
	{
		remove(fileName.c_str());
	}
};

const std::string IniFileTestsFixture::fileContent = "; KEY=IGNORED\nKEY_1=VALUE_1\nIGNORED\nKEY_2=VALUE_2";
const std::string IniFileTestsFixture::fileName = "__testIni.ini";

BOOST_FIXTURE_TEST_SUITE(IniFileTests, IniFileTestsFixture)

BOOST_AUTO_TEST_CASE(ExistingFile)
{
	BOOST_CHECK_EQUAL(cfg.readFromFile(fileName), SUCCESS);
	BOOST_CHECK_EQUAL(cfg["KEY_1"], "VALUE_1");
	BOOST_CHECK_EQUAL(cfg["KEY_2"], "VALUE_2");
	BOOST_CHECK(cfg.find("KEY") == cfg.end());
	BOOST_CHECK(cfg.find("IGNORED") == cfg.end());
}

BOOST_AUTO_TEST_CASE(UnexistingFile)
{
	BOOST_CHECK(cfg.readFromFile("unexisting file.txt") == NO_CONFIG_FILE);
}

BOOST_AUTO_TEST_SUITE_END()
