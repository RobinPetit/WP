#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <server/ErrorCode.hpp>
#include <iostream>

// method called by a new thread only
void chatListening(sf::Uint16 *port, bool *loop)
{
	sf::TcpListener chatListener;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		*port = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		*port = chatListener.getLocalPort();
	while(*loop)
	{
		sf::TcpSocket socket;
		if(chatListener.accept(socket) == sf::Socket::Done)
		{
			std::cout << "communication reçue!" << std::endl;
			/*sf::Packet receivedPacket;
			if(socket.receive(receivedPacket) == sf::Socket::Done)
			{
				std::string friendName;
				std::string friendAddress;
				sf::Uint16 friendPort;
				receivedPacket >> friendName >> friendAddress >> friendPort;
				std::cout << friendName << " wants to chat with you (at " << friendAddress << ", " << friendPort << ")";
			}*/
		}
	}
}
