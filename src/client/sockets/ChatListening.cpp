// SFML headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Time.hpp>
// WizardPoker headers
#include "client/sockets/Client.hpp"
#include "client/ErrorCode.hpp"
#include "common/constants.hpp"
#include "common/Terminal.hpp"
#include "common/sockets/TransferType.hpp"
// std-C++ headers
#include <iostream>
#include <atomic>
#include <string>
#include <cstdlib>

// function called by a new thread only
void Client::chatListening()
{
	//bool volatile _continue = *loop;
	sf::TcpListener chatListener;
	//~ use of a selector to be non-blocking. There may be a better idea
	sf::SocketSelector selector;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		std::cerr << "Unable to listen to arriving chat connections!" << std::endl;
		_chatListenerPort = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		_chatListenerPort = chatListener.getLocalPort();
	std::cout << "waiting for connections on port " << _chatListenerPort << std::endl;
	selector.add(chatListener);
	while(_threadLoop.load())
	{
		// set waiting to 0.1 second so that the loop variable is checked frequently enough
		if(!selector.wait(SOCKET_TIME_SLEEP))
			continue;
		sf::TcpSocket socket;
		if(chatListener.accept(socket) == sf::Socket::Done)
		{
			sf::Packet packet;
			socket.receive(packet);
			TransferType type;
			packet >> type;
			if(type == TransferType::CHAT_PLAYER_IP)
				startChat(socket, packet);
			else
				std::cerr << "Unknown type of message\n";
			std::cin.ignore();
		}
	}
}

void Client::startChat(sf::TcpSocket& socket, sf::Packet& transmission)
{
	sf::Uint32 address;
	sf::Uint16 port;
	std::string otherName, selfName;
	transmission >> address >> port >> selfName >> otherName;
	std::string cmd;
	cmd = _userTerminal.startProgram(
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
