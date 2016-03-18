#ifndef _ABSTRACT_CHAT_
#define _ABSTRACT_CHAT_

// SFML headers
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/NonCopyable.hpp>
// std-C++ headers
#include <string>
#include <thread>
#include <atomic>

class AbstractChat : sf::NonCopyable
{
public:
	/// Constructor
	AbstractChat(const char * const argv[6]);

	/// The function used to display a message written by someone
	/// The method is pure virtual because only child classes can determine how
	/// display text on screen
	/// \param name A string containing the name of the author of the message
	/// (must be _selfName or _friendName)
	/// \param message A string containing the message to dsplay
	virtual void display(const std::string& name, const std::string& message) = 0;

	/// Virtual method allowing child classes to have a determinate behaviour when
	/// both players are eventually connected one to another.
	/// The method is called in start right after getting connected.
	virtual void onConnection();

	/// The method to call to create the discussion
	void start();

	/// Destructor (/!\ must be virtual!)
	virtual ~AbstractChat();

protected:
	////////// Attributes

	/// The name of the user
	std::string _selfName;
	/// The name of the user's friend
	std::string _friendName;

	/// A thread-safe boolean telling whether or not the user wants to keep on chatting
	std::atomic_bool _running;

	/// A thread-safe boolean telling whether or not the friend is still connected
	std::atomic_bool _friendPresence;

	////////// Methods

	/// The function to call after having an input from the user to send to its friend.
	/// Treats special possible inputs (starting by ':')
	/// \param message The text the user entered to send to its friend
	void treatMessage(const std::string& message);

	/// The function to call to end the discussion (and then the program)
	void endDiscussion();

	/// The function called by start to get user's input to send to the friend
	virtual void output() = 0;

private:
	////////// Attributes

	/// The address to connect to
	sf::IpAddress _address;

	/// A string containing "caller" or "callee" depending on the parameters
	std::string _role;

	/// The port to connect to
	sf::Uint16 _remotePort;

	/// The thread waiting for friend's inputs
	std::thread _inputThread;

	/// The socket receiving signals from the friend
	sf::TcpSocket _in;
	/// The socket sending data to the friend
	sf::TcpSocket _out;
	/// The listener socket used to connect to the friend
	sf::TcpListener _listener;

	////////// Methods

	/// The function called for the requester to connect properly
	/// to its friend
	void connectAsCaller();

	/// The function called for the friend being asked to chat
	/// to receive the listening informations from its friend
	void connectAsCallee();

	/// The function called when starting the chat.
	/// Sets up the sockets
	/// \see connectAsCaller
	/// \see connectAsCalle
	void connect();

	/// The function called the the listening thread, awaiting for friend
	/// messages and partiular signals (leaving for example)
	void input();
};

#endif  // _ABSTRACT_CHAT_
