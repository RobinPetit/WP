#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
#include <server/ErrorCode.hpp>
#include <iostream>
#include <atomic>
#include <string>
#include <cstdlib>

static inline std::string chatCommand(sf::Uint32 address, sf::Uint16 port, std::string selfName, std::string otherName);

// function called by a new thread only
void chatListening(sf::Uint16 *port, std::atomic_bool *loop)
{
	//bool volatile _continue = *loop;
	sf::TcpListener chatListener;
	//~ use of a selector to be non-blocking. There may be a better idea
	sf::SocketSelector selector;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		*port = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		*port = chatListener.getLocalPort();
	selector.add(chatListener);
	while(loop->load())
	{
		// set waiting to 1 second so that the loop variable is checked frequently
		if(!selector.wait(sf::seconds(0.1)))
			continue;
		sf::TcpSocket socket;
		if(chatListener.accept(socket) == sf::Socket::Done)
		{
			std::cout << "connection!\n";
			sf::Uint32 address;
			sf::Uint16 port;
			std::string otherName, selfName;
			sf::Packet packet;
			socket.receive(packet);
			packet >> address >> port >> selfName >> otherName;
			system(chatCommand(address, port, selfName, otherName).c_str());
		}
	}
}

static inline std::string chatCommand(sf::Uint32 address, sf::Uint16 port, std::string selfName, std::string otherName)
{
    return "gnome-terminal "
           "-x ./WizardPoker_chat "
           "callee "  // frst parameter is caller or callee
           + std::to_string(address) + " "
           + std::to_string(port) + " "
           + selfName + " "
           + otherName;
}
