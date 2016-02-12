/**
	client main file (entry point)
**/

// SFML headers
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System.hpp>
// std-C++ headers
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <string>
// WizardPoker headers
#include <common/sockets/TransferType.hpp>
#include <common/constants.hpp>

// static functions prototypes
static void input(sf::TcpSocket *inputSocket, const std::atomic_bool *wait, const std::string& otherName);
static inline sf::Packet formatOutputMessage(std::string message);
static inline std::string setBold(std::string message);
static void enterMessages(sf::TcpSocket& out, const std::string& name);

#ifdef __linux__
static inline std::string setBold(std::string message)
{
	return "\033[1m" + message + "\033[0m";
}
#else
static inline std::string setBold(std::string message)
{
	return message;
}
#endif

/// input is the function that is used in the "input" thread for the chat
/// it waits for the other player to send messages and displays them on the screen
/// \param inputSocket A pointer to the socket created to receive the other player's messages
/// \param wait A thread-safe boolean telling whethere the thread needs to keep waiting or has to stop
/// \param otherName the name of the other player
static void input(sf::TcpSocket *inputSocket, const std::atomic_bool *wait, const std::string& otherName)
{
	sf::TcpSocket& in = *inputSocket;  // create reference to simplify reading
	in.setBlocking(false);  // set the socket as non-blocking so that it verifies that connection is still available
	sf::Packet packet;
	// the wait variable is changed in the "main" thread to tell when the program quits and then when to stop the thread
	while(wait->load())
	{
		sf::Socket::Status receivedStatus = in.receive(packet);
		if(receivedStatus == sf::Socket::Done)
		{
			TransferType type;
			std::string message;
			packet >> type;
			if(type == TransferType::CHAT_MESSAGE)
			{
				packet >> message;
				std::cout << setBold(otherName + ": ") << message << std::endl;
			}
			else if(type == TransferType::CHAT_QUIT)
				std::cout << setBold(otherName) << " left the discussion" << std::endl;
			else
				std::cerr << "Wrong transfer\n";
		}
		else if(receivedStatus == sf::Socket::NotReady)  // no data has been sent
			sf::sleep(SOCKET_TIME_SLEEP);  // do not keep the processor and wait a bit
	}
}

/// output is the fucntion where the user writes the messages to be sent to the other player
/// \param out The output socket to send data with
/// \param name The name of the player
static void output(sf::TcpSocket& out, const std::string& name)
{
	std::string message;
	bool loop(true);
	while(loop)
	{
		getline(std::cin, message);
		if(message[0] != ':')
		{
			sf::Packet packet = formatOutputMessage(message);
			std::cout << setBold(name + ": ") << message << std::endl;
			out.send(packet);
		}
		else if(message == ":quit")
		{
			loop = false;
			sf::Packet packet;
			packet << TransferType::CHAT_QUIT;
			out.send(packet);
		}
	}
}

/// formatOutputMessage is a function that's supposed to be inlined and which is used
/// to create a correct transmission packet for the other user
/// \param message A string containing the message to send to the other player
static inline sf::Packet formatOutputMessage(std::string message)
{
	sf::Packet packet;
	packet << TransferType::CHAT_MESSAGE << message;
	return packet;
}

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
	std::atomic_bool running(true);

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
		// the connection to the server is blocking because of the several delays possible
		out.setBlocking(true);
		// transfer in the right order the necessary data to the server
		packet << TransferType::CHAT_PLAYER_IP << selfName << otherName << listener.getLocalPort();
		out.send(packet);
		// get back the other player's address
		out.receive(packet);
		sf::Uint32 calleeAddress;
		packet >> calleeAddress;
		// create the first socket (receiving data from the other player
		listener.accept(in);
		// get the port use to send data to the other player
		in.receive(packet);
		packet >> calleeListeningPort;
		// and finally create the second socket (sending data)
		out.disconnect();
		if(out.connect(in.getRemoteAddress(), calleeListeningPort) != sf::Socket::Done)
			std::cerr << "\tUnable to connect to called friend\n";
		// here, the users can start to discuss
		std::cout << "You are starting a discussion (as " << selfName << ") with " << otherName << std::endl;
	}
	else if(role == "callee")
	{
		// if player is the callee, everything is already done in the chatListening function
		out.connect(address, gameServerPort);
		packet << listener.getLocalPort();
		out.send(packet);
		listener.accept(in);
		std::cout << otherName << " wants to start a discussion with you" << std::endl;
	}
	else
	{
		std::cerr << "unknown role: " << role;
		return EXIT_FAILURE;
	}
	std::thread inputThread(input, &in, &running, otherName);
	// Warning: the `in` socket may not be used in this thread anymore!
	output(out, selfName);
	running.store(false);
	inputThread.join();
	return EXIT_SUCCESS;
}

