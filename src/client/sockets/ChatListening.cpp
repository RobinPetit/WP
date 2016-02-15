// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
// WizardPoker headers
#include <server/ErrorCode.hpp>
#include <common/constants.hpp>
#include <common/Terminal.hpp>
// std-C++ headers
#include <iostream>
#include <atomic>
#include <string>
#include <cstdlib>

// function called by a new thread only
/// chatListening is the function used by the client to make a new thread listening for entring connections
/// (players that want to make a discussion)
/// \param port A pointer to an integer which will contain the port of the listening socket
/// \param loop A thread-safe boolean to tell whether the thread has to keep waiting for connections or has to stop
/// \param terminal An instance of the Terminal class allowing to have a better management of the command system
void chatListening(sf::Uint16 *port, const std::atomic_bool *loop, Terminal terminal)
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
			sf::Uint32 address;
			sf::Uint16 port;
			std::string otherName, selfName;
			sf::Packet packet;
			socket.receive(packet);
			packet >> address >> port >> selfName >> otherName;
			std::string cmd;
			cmd = terminal.startProgram(
				"WizardPoker_chat",
				{
					"callee",
					std::to_string(address),
					std::to_string(port),
					selfName,
					otherName
				});
			system(cmd.c_str());
		}
	}
}
