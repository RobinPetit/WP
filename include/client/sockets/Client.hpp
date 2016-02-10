#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include <thread>

class Client
{
public:
	Client(const std::string& name);

	bool connectToServer(const sf::IpAddress& address, int port);
	bool startConversation(const std::string& playerName);

	void quit();

private:
	sf::TcpSocket _socket;
	sf::Uint16 _chatListenerPort;
	std::string _name;
	std::vector<std::string> _currentConversations;
	bool _isConnected;
	bool _threadLoop;
	std::thread _listenerThread;

	// private methods
	void initListener();
};

#endif // _CONNECTION_HPP_
