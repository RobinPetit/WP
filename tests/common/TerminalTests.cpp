#include <boost/test/unit_test.hpp>
#include "common/Terminal.hpp"

struct TerminalTestsFixture
{
	TerminalTestsFixture()
	{
	}

	~TerminalTestsFixture()
	{
	}
};

BOOST_FIXTURE_TEST_SUITE(TerminalTests, TerminalTestsFixture)

BOOST_AUTO_TEST_CASE(MyMethod)
{
	BOOST_CHECK_EQUAL(1 + 1, 2);
	BOOST_CHECK(1 < 2);
}

BOOST_AUTO_TEST_SUITE_END()
