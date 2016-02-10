#include <SFML/Network.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <common/sockets/TransferType.hpp>

int main(int argc, char **argv)
{
	if(argc != 6)
		return EXIT_FAILURE;
	/* role = "caller" if it must ask the server to tell the other player to connect
	and role = "callee" if it must make a direct connection to the host */
	std::string role = argv[1];
	std::string address = argv[2];
	std::string port = argv[3];
	std::string selfName = argv[4];
	std::string otherName = argv[5];
	sf::TcpSocket out, in;
	sf::TcpListener listener;
	sf::Uint16 p, calleeListeningPort;
	std::stringstream(port) >> p;
	listener.setBlocking(true);
	listener.listen(sf::Socket::AnyPort);
	if(role == "caller")
	{
		out.connect(address, p);
		sf::Packet packet;
		packet << TransferType::CHAT_PLAYER_IP << selfName << otherName << listener.getLocalPort();
		out.send(packet);
		out.receive(packet);
		sf::Uint32 calleeAddress;
		packet >> calleeAddress;
		listener.accept(in);
		in.receive(packet);
		packet >> calleeListeningPort;
		out.connect(in.getRemoteAddress(), calleeListeningPort);
	}
	else if(role == "callee")
	{
		out.connect(address, p);
		sf::Packet packet;
		packet << listener.getLocalPort();
		listener.accept(in);
	}
	else
	{
		std::cerr << "unknown role: " << role;
		return EXIT_FAILURE;
	}
	int n;
	std::cout << role;
	std::cin >> n;
	return EXIT_SUCCESS;
}
