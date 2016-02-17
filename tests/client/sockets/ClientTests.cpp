#include <boost/test/unit_test.hpp>
// \FIXME the include below leads to compilation error
//#include "client/sockets/Client.hpp"

struct ClientTestsFixture
{
	ClientTestsFixture()
	{
	}

	~ClientTestsFixture()
	{
	}
};

BOOST_FIXTURE_TEST_SUITE(ClientTests, ClientTestsFixture)

BOOST_AUTO_TEST_CASE(MyMethod)
{
	BOOST_CHECK_EQUAL(1 + 1, 2);
	BOOST_CHECK(1 < 2);
}

BOOST_AUTO_TEST_SUITE_END()
