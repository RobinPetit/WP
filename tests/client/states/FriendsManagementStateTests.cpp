#include <boost/test/unit_test.hpp>
#include "client/states/FriendsManagementState.hpp"

struct FriendsManagementStateTestsFixture
{
    FriendsManagementStateTestsFixture()
    {
    }

    ~FriendsManagementStateTestsFixture()
    {
    }
};

BOOST_FIXTURE_TEST_SUITE(FriendsManagementStateTests, FriendsManagementStateTestsFixture)

BOOST_AUTO_TEST_CASE(MyMethod)
{
    BOOST_CHECK_EQUAL(1 + 1, 2);
    BOOST_CHECK(1 < 2);
}

BOOST_AUTO_TEST_SUITE_END()
