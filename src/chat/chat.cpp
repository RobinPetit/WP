#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
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
	sf::IpAddress address = argv[2];
	sf::Uint16 gameServerPort;
	std::stringstream(argv[3]) >> gameServerPort;
	std::string selfName = argv[4];
	std::string otherName = argv[5];

	sf::Packet packet;  // used to exchange data (I/O)
	sf::TcpSocket out, in;  // two separated sockets for IO operations
	sf::TcpListener listener;  // listener helping to get the entring connection
	sf::Uint16 calleeListeningPort;  // port which is asked by the out (alias `server`) socket
	/* the listener is set as blocking because it is waiting for a precise
	amount of connections that are well defined */
	listener.setBlocking(true);
	listener.listen(sf::Socket::AnyPort);
	if(role == "caller")
	{
		// out here represents the game server
		out.connect(address, gameServerPort);
		// transfer in the right order the necessary data to the server
		packet << TransferType::CHAT_PLAYER_IP << selfName << otherName << listener.getLocalPort();
		out.send(packet);
		// get back the other player's address
		out.receive(packet);
		std::string calleeAddress;
		packet >> calleeAddress;
		// create the first socket (receiving data from the other player
		listener.accept(in);
		// get the port use to send data to the other player
		in.receive(packet);
		packet >> calleeListeningPort;
		// and finally create the second socket (sending data)
		out.connect(in.getRemoteAddress(), calleeListeningPort);
		// here, the users can start to discuss
	}
	else if(role == "callee")
	{
		// if player is the callee, everything is already done in the chatListening function
		out.connect(address, gameServerPort);
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
