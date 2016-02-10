#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <server/ErrorCode.hpp>
#include <iostream>

// function called by a new thread only
void chatListening(sf::Uint16 *port, bool *loop)
{
	bool volatile _continue = *loop;
	sf::TcpListener chatListener;
	if(chatListener.listen(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		*port = 0;
		exit(UNABLE_TO_LISTEN);
	}
	else
		*port = chatListener.getLocalPort();
	while(_continue)
	{
		sf::TcpSocket socket;
		if(chatListener.accept(socket) == sf::Socket::Done)
		{

		}
	}
}
