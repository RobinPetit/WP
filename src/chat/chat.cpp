/**
	client main file (entry point)
**/

// SFML headers
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Sleep.hpp>
// std-C++ headers
#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include <fstream>
#include <mutex>
// WizardPoker headers
#include "common/sockets/TransferType.hpp"
#include "common/constants.hpp"

std::mutex loggerMutex;
std::ofstream logFile;
std::ostream& outStream = std::cout;  // define the text output is stdout

// static functions prototypes
static void input(sf::TcpSocket *inputSocket, const std::atomic_bool *wait, const std::string& otherName, std::atomic_bool *presence);
static inline sf::Packet formatOutputMessage(const std::string& message);
static inline std::string setBold(const std::string& message);
static void output(sf::TcpSocket& out, const std::string& name, const std::atomic_bool &presence);
static inline void endDiscussion(bool& running, sf::TcpSocket& socket, bool sending);
static void display(std::ostream& outputStream, const std::string& name, const std::string& message, bool setAsComment=false);
static inline std::string getDiscussionFileName(const std::string& otherName, const std::string& name);
static void restoreOldDiscussion(const std::string& otherName, const std::string& name);

#ifdef __linux__
static inline std::string setBold(const std::string& message)
{
	return "\033[1m" + message + "\033[0m";
}
#else
static inline std::string setBold(const std::string& message)
{
	return message;
}
#endif

/// display is the function that outputs the received (or sent) message and logs it
/// \param outputStream The stream to write the message (std::cout if terminal)
/// \param name The name of the person sending the message
/// \param message The message to be displayed
static void display(std::ostream& outputStream, const std::string& name, const std::string& message, bool setAsComment)
{
	std::string displayedMessage((setAsComment ? "// " : "") + setBold(name) + ": " + message + "\n");
	loggerMutex.lock();
	outputStream << displayedMessage;
	logFile << displayedMessage;
	loggerMutex.unlock();
}

/// endDiscussion is a fucntion that tells to stop looping waiting for messages and send
/// to the other player that this player leaves the discussion
/// \param running Set to false
/// \param socket The output socket where CHAT_QUIT is send to
/// \param sending True if necessary to send to the other player (still connected) false otherwise (already disconnected)
static inline void endDiscussion(bool& running, sf::TcpSocket& socket, bool sending)
{
	running = false;
	if(!sending)
		return;
	sf::Packet packet;
	packet << TransferType::CHAT_QUIT;
	socket.send(packet);
}

/// input is the function that is used in the "input" thread for the chat
/// it waits for the other player to send messages and displays them on the screen
/// \param inputSocket A pointer to the socket created to receive the other player's messages
/// \param wait A thread-safe boolean telling whethere the thread needs to keep waiting or has to stop
/// \param otherName the name of the other player
/// \param presence A thread-safe boolean telling whether the other player has left or not
static void input(sf::TcpSocket *inputSocket, const std::atomic_bool *wait, const std::string& otherName, std::atomic_bool *presence)
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
				display(outStream, otherName, message);
			}
			else if(type == TransferType::CHAT_QUIT)
			{
				display(outStream, otherName, "left the discussion");
				presence->store(false);
			}
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
/// \param presence A thread-safe boolean telling whether the other player hasleft or not
static void output(sf::TcpSocket& out, const std::string& name, const std::atomic_bool &presence)
{
	std::string message;
	bool loop(true);
	while(loop)
	{
		getline(std::cin, message);
		if(message[0] != ':')
		{
			if(presence.load())
			{
				sf::Packet packet = formatOutputMessage(message);
				out.send(packet);
			}
			display(outStream, name, message, !presence.load());
		}
		else if(message == ":quit")
			endDiscussion(loop, out, presence.load());
	}
}

/// restoreOldDiscussion is a function that reads from the log discussion file
/// and displays the content of the previous discussions with this person
/// \param otherName A string containing the name of the other player
/// \param name A strign containing the player's name
static void restoreOldDiscussion(const std::string& otherName, const std::string& name)
{
	std::ifstream backup;
	backup.open(getDiscussionFileName(otherName, name));
	if(!backup)
		return;
	// backup does not need to be thread tested with mutex::lock because no other thread has yet been created
	char *buffer;
	backup.seekg(0, backup.end);
	std::ifstream::pos_type length = backup.tellg();
	std::cout << "size is: " << length << std::endl;
	backup.seekg(0, backup.beg);
	buffer = new char[length];
	backup.read(buffer, static_cast<std::streamsize>(length));
	outStream << buffer;
	delete[] buffer;
	backup.close();
}

/// formatOutputMessage is a function that's supposed to be inlined and which is used
/// to create a correct transmission packet for the other user
/// \param message A string containing the message to send to the other player
static inline sf::Packet formatOutputMessage(const std::string& message)
{
	sf::Packet packet;
	packet << TransferType::CHAT_MESSAGE << message;
	return packet;
}

/// getDiscussionFileName is a function that's supposed to be inlined and which is used
/// to find the right file name where the discussion has been/will be stored
/// \param othername A string containing the other player's name
/// \param name A strign containing the player's name
static inline std::string getDiscussionFileName(const std::string& otherName, const std::string& name)
{
	return CHAT_LOGGERS_DIR + name + "_" + otherName + ".txt";
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
		TransferType type;
		packet >> type;
		if(type == TransferType::FAILURE)
			throw std::runtime_error("Unable to start discussion");
		sf::Uint32 calleeAddress;
		packet >> calleeAddress;
		// create the first socket (receiving data from the other player
		listener.accept(in);
		// get the port use to send data to the other player
		in.receive(packet);
		sf::Uint16 calleeListeningPort;  // port which is asked by the out (alias `server`) socket
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
		throw std::runtime_error("unknown role: " + role);
	std::atomic_bool presence(true);
	restoreOldDiscussion(otherName, selfName);
	// open the file in append mode only after it has been open in read-only mode
	logFile.open(getDiscussionFileName(otherName, selfName), std::ios_base::out | std::ios_base::trunc | std::ios_base::app);
	std::thread inputThread(input, &in, &running, otherName, &presence);
	// Warning: the `in` socket shall not be used in this thread anymore!
	output(out, selfName, presence);
	// code to quit properly (frees the memory)
	running.store(false);
	inputThread.join();
	// close the file when the input thread is over: be sure it does not need to log something anymore
	logFile.close();
	return EXIT_SUCCESS;
}

