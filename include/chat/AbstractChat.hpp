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
	AbstractChat(const char * const argv[6]);

	virtual void display(const std::string& name, const std::string& message) = 0;

	virtual void output() = 0;

	virtual void onConnection();

	void start();

	virtual ~AbstractChat();

protected:
	std::string _role;
	sf::IpAddress _address;
	std::string _selfName;
	std::string _friendName;
	std::atomic_bool _running;
	sf::Uint16 _gameServerPort;

	std::thread _inputThread;

	sf::TcpSocket _in;
	sf::TcpSocket _out;
	sf::TcpListener _listener;

	std::atomic_bool _friendPresence;

	void treatMessage(const std::string& message);

	void endDiscussion();

private:
	void connectAsCaller();
	void connectAsCallee();

	void connect();

	void input();
};

#endif  // _ABSTRACT_CHAT_
