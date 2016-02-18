#include <boost/test/unit_test.hpp>
#include "client/AbstractState.hpp"

class TestState : public AbstractState
{
	public:
		TestState(StateStack& stateStack, Client& c):
			AbstractState(stateStack, c),
			_fHasBeenCalled{false},
			_gHasBeenCalled{false}
		{
			addAction("f", &TestState::f);
			addAction("g", &TestState::g);
		}

		bool _fHasBeenCalled, _gHasBeenCalled;

	private:
		void f()
		{
			_fHasBeenCalled = true;
		}

		void g()
		{
			_gHasBeenCalled = true;
		}
};

struct AbstractStateTestsFixture
{
	Client c;
	StateStack stack;
	TestState testState;

	AbstractStateTestsFixture():
		c{},
		stack{c},
		testState{stack, c}
	{
	}

	~AbstractStateTestsFixture()
	{
	}
};

BOOST_FIXTURE_TEST_SUITE(AbstractStateTests, AbstractStateTestsFixture)

BOOST_AUTO_TEST_CASE(MenuCallback)
{
	testState.handleInput("0");
	BOOST_CHECK(testState._fHasBeenCalled);
	testState.handleInput("1");
	BOOST_CHECK(testState._gHasBeenCalled);
}

BOOST_AUTO_TEST_SUITE_END()

