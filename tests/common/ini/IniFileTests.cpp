#include <boost/test/unit_test.hpp>
#include "common/ini/IniFile.hpp"

struct IniFileTestsFixture
{
    IniFileTestsFixture()
    {
    }

    ~IniFileTestsFixture()
    {
    }
};

BOOST_FIXTURE_TEST_SUITE(IniFileTests, IniFileTestsFixture)

BOOST_AUTO_TEST_CASE(MyMethod)
{
    BOOST_CHECK_EQUAL(1 + 1, 2);
    BOOST_CHECK(1 < 2);
}

BOOST_AUTO_TEST_SUITE_END()
