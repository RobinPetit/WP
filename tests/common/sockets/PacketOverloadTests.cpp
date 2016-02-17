#include <vector>
#include <boost/test/unit_test.hpp>
#include "common/sockets/PacketOverload.hpp"
#include <SFML/Network/Packet.hpp>

template <typename T>
static bool verify(const std::vector<T>& a)
{
	std::vector<T> b;
	sf::Packet packet;
	packet << a;
	packet >> b;
	return a == b;
}

BOOST_AUTO_TEST_CASE(Packing)
{
	std::vector<int> ivect{5, 4, 6,8 , 2, 1, -2, 33};
	std::vector<double> dvect{0.2, -.66, 3.14};
	std::vector<std::string> svect{"When", "I get", "to the" "bottom", "I", "go back", "to the", "top of the slide"};
	BOOST_CHECK(verify(ivect));
	BOOST_CHECK(verify(dvect));
	BOOST_CHECK(verify(svect));
}
