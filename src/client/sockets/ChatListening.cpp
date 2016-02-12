// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
// WizardPoker headers
#include <server/ErrorCode.hpp>
#include <common/constants.hpp>
// std-C++ headers
#include <iostream>
#include <atomic>
#include <string>
#include <cstdlib>

// static functions prototypes
static inline std::string chatCommand(sf::Uint32 address, sf::Uint16 port, std::string selfName, std::string otherName);

// function called by a new thread only
/// chatListening is the function used by the client to make a new thread listening for entring connections
/// (players that want to make a discussion)
/// \param port A pointer to an integer which will contain the port of the listening socket
/// \param loop A thread-safe boolean to tell whether the thread has to keep waiting for connections or has to stop
void chatListening(sf::Uint16 *port, const std::atomic_bool *loop)
{
	//bool volatile _continue = *loop;
	sf::TcpListener chatListener;
	//~ use of a selector to be non-blocking. There may be a better idea
	sf::SocketSelector selector;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cerr << "Unable to listen to arriving chat connections!" << std::endl;
		*port = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		*port = chatListener.getLocalPort();
        std::cout << "waiting for connections on port " << *port << std::endl;
	selector.add(chatListener);
	while(loop->load())
	{
		// set waiting to 0.1 second so that the loop variable is checked frequently enough
		if(!selector.wait(SOCKET_TIME_SLEEP))
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

/// chatCommand is a function that's supposed to be inlined which make the terminal command to start the chat
/// \param address An integer representing the address of the player who wants to chat
/// \param port An integer representing the port the other player is listening on
/// \param selfName The name of the called user (self)
/// \param otherName The name of the caller user (the other one)
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
