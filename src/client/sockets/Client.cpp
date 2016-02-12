// SFML headers
#include <SFML/System/Sleep.hpp>
#include <SFML/Network/IpAddress.hpp>
// WizardPoker headers
#include <client/sockets/Client.hpp>
#include <common/constants.hpp>
#include <common/sockets/TransferType.hpp>
// std-C++ headers
#include <iostream>
#include <thread>
#include <cstdlib>

extern void chatListening(sf::Uint16 *port, const std::atomic_bool *loop);

Client::Client(const std::string& name):
	_socket(),
	_currentConversations(),
	_isConnected(false),
	_chatListenerPort(0),
	_threadLoop(0),
	_serverAddress(),
	_serverPort(0),
	_userTerminal()
{
	// forces the name to not be larger than MAX_NAME_LENGTH
	_name = (name.size() < MAX_NAME_LENGTH) ? name : name.substr(0, MAX_NAME_LENGTH);
	if(!_userTerminal.hasKnownTerminal())
		std::cout << "Warning: as no known terminal has been found, chat is disabled" << std::endl;
}

bool Client::connectToServer(const sf::IpAddress& address, sf::Uint16 port)
{
	// if client is already connected to a server, do not try to re-connect it
	if(_isConnected)
		return false;
	_serverAddress = address;
	_serverPort = port;
	initListener();  // creates the new thread which listens for entring chat conenctions
	// if connection does not work, don't go further
	if(_socket.connect(address, port) != sf::Socket::Done)
		return false;
	sf::sleep(SOCKET_TIME_SLEEP);  // wait a quarter second to let the listening thread init the port
	sf::Packet packet;
	packet << TransferType::GAME_CONNECTION  // precise
	       << _name  // do not forget the '\0' character
	       << static_cast<sf::Uint16>(_chatListenerPort);
	if(_socket.send(packet) != sf::Socket::Done)
		return false;
	_isConnected = true;
	return true;
}

bool Client::startConversation(const std::string& playerName)
{
	// rest assured the client is connected to a server before trying to access it
	if(!_isConnected || !_userTerminal.hasKnownTerminal())
		return false;
	std::string cmd;
	cmd = _userTerminal.getTerminalName() + " "
	      "-x ./WizardPoker_chat "
	      "\"caller\" "  // parameter 1 is caller/callee
	      "\"" + _serverAddress.toString() + "\" "  // parameter 2 is the address to connect to
	      "\"" + std::to_string(_serverPort) + "\" "  // parameter 3 is the port to connect to
	      "\"" + _name + "\" "  // parameter 4 is caller's name
	      "\"" + playerName + "\"";  // parameter 5 is callee's name
	      // there is not more parameters!
	system(cmd.c_str());
	return true;
}

void Client::initListener()
{
	_threadLoop.store(true);
	_listenerThread = std::thread(chatListening, &_chatListenerPort, &_threadLoop);
}

void Client::quit()
{
	_threadLoop.store(false);
	_listenerThread.join();
}
