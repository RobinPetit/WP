#include <boost/test/unit_test.hpp>
#include "client/states/MainMenuState.hpp"

struct MainMenuStateTestsFixture
{
    MainMenuStateTestsFixture()
    {
    }

    ~MainMenuStateTestsFixture()
    {
    }
};

BOOST_FIXTURE_TEST_SUITE(MainMenuStateTests, MainMenuStateTestsFixture)

BOOST_AUTO_TEST_CASE(MyMethod)
{
    BOOST_CHECK_EQUAL(1 + 1, 2);
    BOOST_CHECK(1 < 2);
}

BOOST_AUTO_TEST_SUITE_END()